/* file    	Makefile
 * copyright 	Copyright (c) 2012 Toradex AG
 * author  	Max Krummenacher <max.krummenacher@toradex.com>
 * brief   	A test programm which draws a grid on /dev/fb0 /dev/fb1
 *          the grid pattern can be set, so that the two screens look different
 */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <linux/fb.h>
#include <sys/ioctl.h>

int fb;
unsigned no_row;
unsigned no_columns;
unsigned depth;
unsigned currentrow;
struct fb_var_screeninfo vinfo;

void fb_write_line(unsigned rowpattern)
{
	unsigned i;
	unsigned j;
	unsigned pixel;
	for(i=0; i<vinfo.xres; i++)
	{
		pixel = (rowpattern & (1<<(i%32))) > 0 ? 0xffffffff : 0;
		for(j=0; j<(vinfo.bits_per_pixel/8); j++)
		{
			write(fb, &pixel, 1);
		}
	}
	currentrow++;
}

//returns 1 when screen is painted
unsigned fb_write_32(unsigned rowpattern, unsigned columnpattern)
{
	unsigned i;
	for(i=0; i<32; i++)
	{
		fb_write_line( (columnpattern & (1<<i)) > 0 ? 0xffffffff : rowpattern);
		if (currentrow > vinfo.yres)
			break;
	}
	if(vinfo.yres > 2000)
		return 1;
	return (currentrow > vinfo.yres) ? 1 : 0;
}

int main(void)
{
	char *fb_dev_file = "/dev/fb0";
	fb = open(fb_dev_file, O_RDWR);
	if (fb < 0) {
		printf("Could not open %s, exiting...\n", fb_dev_file);
		return 1;
	}
	currentrow = 0;

	if (ioctl(fb, FBIOGET_VSCREENINFO, &vinfo)) {
		printf("Could not get screen info %s, exiting...\n", fb_dev_file);
		close(fb);
		return 1;
	}
	printf("fb %s is of size x: %u, y: %u, depth %u\n", fb_dev_file, vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

	while (fb_write_32(0x80018001, 0x80018001) == 0);
	printf("Painted %u lines\n\n", currentrow);
	close(fb);


	fb_dev_file = "/dev/fb1";
	fb = open(fb_dev_file, O_RDWR);
	if (fb < 0) {
		printf("Could not open %s, exiting...\n", fb_dev_file);
		return 1;
	}
	currentrow = 0;

	if (ioctl(fb, FBIOGET_VSCREENINFO, &vinfo)) {
		printf("Could not get screen info %s, exiting...\n", fb_dev_file);
		close(fb);
		return 1;
	}
	printf("fb %s is of size x: %u, y: %u, depth %u\n", fb_dev_file, vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

	while (fb_write_32(0xf000000f, 0xf000000f) == 0);
	printf("Painted %u lines\n\n", currentrow);
	close(fb);


	return 0;
}
