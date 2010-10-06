#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os, re
import pygtk, gtk
import sched, time

class ImagesExample:
	def close_application(Self, widget, event, data=None):
		gtk.main_quit()
		return False

	def renderRadar(filename):
		return True

	def __init__(self):
		scheduler = sched.scheduler(time.time, time.sleep)

		window = gtk.Window()
		window.set_default_size(600, 750)
		#window.connect('destroy', gtk.main_quit)
		window.connect('delete_event', self.close_application)
		window.set_border_width(5)
		window.set_title("radarwatch")

		files = []
		for file in os.listdir('radar'):
			if re.match("DIX_[0-9]{8}_[0-9]{4}_NCR.gif", file):
				files.append(file)

		files.sort()
		print files

		#radarImage = gtk.gdk.pixbuf_new_from_file("radar/DIX_20090802_1554_NCR.gif")
		radarImage = gtk.gdk.pixbuf_new_from_file("radar/" + files[1])
		radarImage2 = gtk.gdk.pixbuf_new_from_file("radar/" + files[2])
		topoOverlay = gtk.gdk.pixbuf_new_from_file("overlays/DIX_Topo_Short.jpg")
		countyOverlay = gtk.gdk.pixbuf_new_from_file("overlays/DIX_County_Short.gif")
		countyOverlay.composite(topoOverlay, 0, 0, topoOverlay.props.width, topoOverlay.props.height, 0, 0, 1.0, 1.0, gtk.gdk.INTERP_HYPER, 255)
		radarImage.composite(topoOverlay, 0, 0, topoOverlay.props.width, topoOverlay.props.height, 0, 0, 1.0, 1.0, gtk.gdk.INTERP_HYPER, 255)
		radarImage2.composite(topoOverlay, 0, 0, topoOverlay.props.width, topoOverlay.props.height, 0, 0, 1.0, 1.0, gtk.gdk.INTERP_HYPER, 255)
		#radar1.set_from_file("radar/DIX_20090802_1554_NCR.gif")
		#radar1.show()

		radarComposite = gtk.image_new_from_pixbuf(topoOverlay)
		radarComposite.show()

		topFrame = gtk.Frame()
		topFrame.set_label_align(0.0, 0.5)
		topFrame.show()
		topFrame.add(radarComposite)

		bottomFrame = gtk.Frame()
		bottomFrame.set_label(" Configuration ")
		bottomFrame.show()

		align = gtk.Alignment(0.5, 0.0, 0.0, 0.0)
		align.add(topFrame)
		align.show()

		vbox = gtk.VBox(False, 2)
		vbox.set_border_width(2)
		vbox.show()
		vbox.pack_start(align, False, False, 5)
		vbox.pack_start(bottomFrame, False, False, 5)

		#window.add(image)
		#window.add(align)
		window.add(vbox)

		window.show()

def main():
	gtk.main()
	return 0

if __name__ == "__main__":
	ImagesExample()
	main()
