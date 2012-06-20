#!/usr/bin/env python
# -*- coding: utf-8 -*-

########################################################################################
#
#	Copyright 2010 Anthony DeChiaro
#
#	This file is part of radarWatch.
#
#	radarWatch is free software; you can redistribute it and/or modify
#	it under the terms of the GNU General Public License as published by
#	the Free Software Foundation; either version 2 of the License, or
#	any later version.
#
#	radarWatch is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU General Public License for more details.
#
#	You should have received a copy of the GNU General Public License
#	along with radarWatch; if not, write to the Free Software
#	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#
#########################################################################################

#:
# radar class
# -data-source agnostic: process GIF/RAW NetCDF data
# -analyze images, add info to DB
# -fuzzy logic on storm cells?
#
#
# radar image list class
# -instanciate on radar image dir
# -process by default
#
#
#
#
#

#import required files
import sys
#import ftplib
#import argparse
from optparse import OptionParser

#import gobject
#import shutil
import os
#import os.path
import platform
import time
import re
import string
#import urllib

from datetime import datetime

import sqlite3
import zmq

# check for compatable python version
pythonver = sys.version_info
if pythonver[0] < 2 and pythonver[1] < 4:
	print "This application requires"
	print "Python version 2.4.x or better"
	print "to run."
	sys.exit(1)

# Import GTK and GTK.Glade and check GTK version
#try:
#	import pygtk
#	pygtk.require("2.0")
#except:
#	pass
#try:
#	import gtk
#	import gtk.glade
#except:
#	print "You need to install pyGTK or GTKv2"
#	print "or set your PYTHONPATH correctly "
#	print "to run"
#	sys.exit(1)
#
## check for pyGTK version
#pygtkver = gtk.pygtk_version
#if pygtkver[0] < 2 and pygtkver[1] < 6:
#	print "This application requires pyGTK+"
#	print "version 2.6.x or better to run...  bofa"
#	sys.exit(1)
#

radarTypes = { "Base Reflectivity" : "N0R", \
					"Base Reflectivity (LR)" : "N0Z", \
					"Composite Reflectivity" : "NCR", \
					"Base Velocity" : "N0V", \
					"Storm Relative Motion" : "N0S", \
					"Storm Total Precip" : "NTP", \
					"Storm Precip (1 hr)" : "N1P" }

radarPath = "../radar"
overlayPath = "../overlays"

RADAR_WIDTH = 600
RADAR_HEIGHT = 550
#INTERPOLATION_TYPE = gtk.gdk.INTERP_NEAREST

# Begin code
##class radarWatch:
#
#	def __init__(self):
#		##get install path
#		#f = open(os.path.normpath('/etc/radarWatch/here'))
#		#self.inspath = str(f.readline()).strip()
#		#f.close()
#		##create config directory
#		#if not os.path.isdir(os.path.expanduser(os.path.normpath('~/.pyradar/'))):
#			#os.mkdir(os.path.expanduser(os.path.normpath('~/.pyradar')))
#		#if not os.path.isdir(os.path.expanduser(os.path.normpath('~/.pyradar/images/'))):
#			#os.mkdir(os.path.expanduser(os.path.normpath('~/.pyradar/images')))
#		#Linux Setup
#		self.gladefile = os.path.normpath('./pyradar.glade')
#		try:
#			f = open(os.path.expanduser(os.path.normpath('./radarWatch.gif')))
#			haveFile = 1
#			f.close()
#		except:
#			haveFile = 0
#		#if haveFile == 0:
#			#shutil.copy(os.path.normpath(self.inspath+'/lib/radarWatch/images/radarWatch.gif'),os.path.expanduser(os.path.normpath('~/.pyradar/images/radarWatch.gif')))
#		haveFile = 0
#		#try:
#			#f = open(os.path.expanduser(os.path.normpath('~/.pyradar/NOAA-Stations.csv')))
#			#haveFile = 1
#			#f.close()
#		#except:
#			#haveFile = 0
#		#if haveFile == 0:
#				#shutil.copy(os.path.normpath(self.inspath+'/lib/radarWatch/NOAA-Stations.csv'),os.path.expanduser(os.path.normpath('~/.pyradar/NOAA-Stations.csv')))
#		#Setup widget tree
#		self.wTree = gtk.glade.XML(self.gladefile, "radardialog")
#		#self.dialog = gtk.glade.XML(self.gladefile, "window2")
#
#		#connect signal handlers
#		sig = { "on_window1_delete_event" : self.quit, \
#				"on_window1_destroy_event" : self.quit, \
#				"on_radarstation_changed" : self.newStationType, \
#				"on_radartype_changed" : self.newStationType, \
#				#"on_15minbutton_toggled" : self.newTimer, \
#				#"on_30minbutton_toggled" : self.newTimer, \
#				#"on_60minbutton_toggled" : self.newTimer, \
#				"on_stormbutton_clicked" : self.displayStormSelectDialog, \
#				"on_delay_value_changed" : self.delayChanged, \
#				"on_opacity_value_changed" : self.opacityChanged, \
#				"on_loopbutton_toggled" : self.loopToggled, \
#				"on_overlay_toggled" : self.renderOverlays, \
#				"on_firstbutton_clicked" : self.goFirstFrame, \
#				"on_reversebutton_clicked" : self.goPreviousFrame, \
#				"on_playbutton_clicked" : self.startAnimation, \
#				"on_stopbutton_clicked" : self.stopAnimation, \
#				"on_skipbutton_clicked" : self.goNextFrame, \
#				"on_lastbutton_clicked" : self.goLastFrame, \
#				#"on_radarimage1_release_event" : self.imageClicked, \
#				#"on_radarimage1_buttonpress_event" : self.imageClicked
#		}
#		self.wTree.signal_autoconnect(sig)
#
#		self.firstLoop = 1
#		self.displayTimer = 0
#		self.playing = False
#		self.lastfile = ""
#
#		self.dataInit = False
#		self.radarVisible = False
#
#		self.radarFiles = []
#		self.radarImages = []
#
#		self.topoOverlay = 0
#		self.countyOverlay = 0
#		self.frameLimit = 1000
#		self.opacity = 255
#
#		self.wTree.get_widget("topocheck").set_active(1)
#		self.wTree.get_widget("countycheck").set_active(1)
#
#		self.radarview = self.wTree.get_widget("radarimage1")
#		self.radarview.set_from_file(os.path.expanduser(os.path.normpath('./images/radarWatch.gif')))
#
#		eventbox = self.wTree.get_widget("eventbox1")
#		#self.radarview.add_events(gtk.gdk.BUTTON_PRESS_MASK)
#		#self.radarview.add_events(gtk.gdk._2BUTTON_PRESS)
#		#self.radarview.connect("event", self.imageClicked)
#		eventbox.connect("button-press-event", self.imageClicked)
#		eventbox.set_events(gtk.gdk.BUTTON_PRESS_MASK )
#							#gtk.gdk.BUTTON_RELEASE_MASK )
#							#EXPOSE
#
#		self.sizeInit = False
#		self.zoomX = 0
#		self.zoomY = 0
#		self.zoomWidth = 0
#		self.zoomHeight= 0
#		self.radarWidth = 0
#		self.radarHeight = 0
#
#		self.populateStationList()
#		#self.wTree.get_widget("radartype").set_active(0)
#		self.wTree.get_widget("delayhscale").set_value(250)
#		self.loadConfig()
#
#
#	def quit(self, widget, data=None):
#		gtk.main_quit()
#		#self.writeLastConfig()
#
#
#	def loadConfig(self):
#		try:
#			config = open(os.path.expanduser(os.path.normpath('./.pyradar')))
#			haveConfig = True
#		except:
#			haveConfig = False
#
#		if haveConfig:
#			self.settings = {}
#			line = str(config.readline())
#			while line != "":
#					arr = line.split('=')
#					configcat = str(arr[0]).rstrip()
#					configdat = str(arr[1]).rstrip()
#					self.settings[configcat] = configdat
#					line = str(config.readline())
#
#			#place last station at top of list and select
#			ourStation = int(self.settings['stationID'])
#  			self.wTree.get_widget("radarstation").set_active(ourStation)
#
#  			#select last radar station
#  			try:
#  				self.wTree.get_widget("radartype").set_active(int(self.settings['radarType']))
#  			except:
#  				self.wTree.get_widget("radartype").set_active(0)
#			#self.newStation()
#
#			#check last loop status and select
#			#if self.settings['is_looping'] == '1':
#				#self.wTree.get_widget("loopbutton").set_active(True)
#
#
#	def populateStationList(self):
#		# Read in file
#		try:
#			f = open(os.path.expanduser(os.path.normpath('./NOAA-Stations.csv')))
#			self.stations = []
#			x = str(f.readline())
#			while x != "":
#				self.stations.append(x)
#				x = str(f.readline())
#			f.close
#		except:
#			self.wTree.get_widget("label3").set_text('ERROR: Unable to open NOAA Radar Station File')
#
#		# Fill the entries
#		self.store = gtk.ListStore(gobject.TYPE_STRING)
#		for x in self.stations:
#			y = x.split(',')
#			self.store.append([y[0]])
#
#		# Configure the widgets
#		combo = self.wTree.get_widget("radarstation")
#		combo.set_model(self.store)
#		cell = gtk.CellRendererText()
#		combo.pack_start(cell, True)
#		combo.add_attribute(cell,'text',0)
#
#
#	def getStationID(self, station):
#		stID = ""
#		for x in self.stations:
#			y = x.split(',')
#			if station == y[0]:
#				stID = y[1].rstrip()
#
#		return stID
#
#
#	def opacityChanged(self=None, widget=None, param1=None):
#		alpha = int(self.wTree.get_widget("opacityhscale").get_value())
#		print 'using new alpha %d' % (alpha)
#		self.opacity = alpha
#		self.displayRadarImage()
#
#
#	def delayChanged(self=None, widget=None, param1=None):
#		if self.playing:
#			timerLen = int(self.wTree.get_widget("delayhscale").get_value())
#			#print 'Updating delay value :  ' + str(timerLen)
#			if self.displayTimer:
#				gobject.source_remove(self.displayTimer)
#			self.displayTimer = 0
#			self.displayTimer = gobject.timeout_add(timerLen, self.displayRadarImage)
#
#
#	def newStationType(self=None, widget=None, data=None):
#		newRS = str(self.wTree.get_widget("radarstation").get_active_text())
#		self.stID = self.getStationID(newRS)[1:]
#		if self.stID == None:
#			return
#		self.rdType = str(self.wTree.get_widget("radartype").get_active_text())
#		if self.rdType == 'None':
#			return
#		self.radarType = radarTypes[self.rdType]
#
#		if self.stID != "":
#			self.getRadarFiles()
#
#			self.firstLoop = 1
#			self.looppos = 0
#
#			self.showLatest()
#
#
#	def getOverlay(self, type):
#		overlayFNs = { "topo" : "_Topo_Short.jpg", \
#					"county" : "_County_Short.gif", \
#					"highway" : "_Highways_Short.gif", \
#					"city" : "_City_Short.gif" }
#
#		overlayURLs = { "topo" : "http://radar.weather.gov/ridge/Overlays/Topo/Short/", \
#					"county" : "http://radar.weather.gov/ridge/Overlays/County/Short/", \
#					"highway" : "http://radar.weather.gov/ridge/Overlays/Highways/Short/", \
#					"city" : "http://radar.weather.gov/ridge/Overlays/Cities/Short/" }
#
#		overlayFN = string.upper(self.stID) + overlayFNs[type]
#		overlayLocal = overlayPath + os.sep + overlayFN
#		overlayURL = overlayURLs[type] + overlayFN
#
#		try:
#			overlay = gtk.gdk.pixbuf_new_from_file(overlayLocal)
#		except:
#			try:
#				overlayFile = open(overlayLocal, "wb")
#				socket = urllib.urlopen(overlayURL, None, None)
#				print 'Cannot find overlay %s locally,\nDownloading %s' % (overlayLocal, overlayURL)
#			except Exception, e:
#				print "Error downloading overlay file %s\nerror: %s" % (overlayLocal, e)
#
#			bytesRead = 0
#			for line in socket:
#				try:
#					bytesRead += len(line)
#					print "%d bytes read..." % (bytesRead)
#					overlayFile.write(line)
#				except Exception, e:
#					print "Error downloading %s: %s" % (overlayURL, e)
#
#			socket.close()
#			overlayFile.close()
#
#			try:
#				overlay = gtk.gdk.pixbuf_new_from_file(overlayLocal)
#			except Exception, e:
#				print "Error opening overlay file %s\nerror: %s" % (overlayLocal, e)
#				return False
#
#		return overlay
#
#
#	def renderOverlays(self, widget=None):
#		if not self.dataInit: return
#
#		overlayImage = gtk.gdk.Pixbuf(gtk.gdk.COLORSPACE_RGB, True, 8, RADAR_WIDTH, RADAR_HEIGHT)
#
#		if self.wTree.get_widget("topocheck").get_active():
#			self.topoOverlay = self.getOverlay("topo")
#			self.topoOverlay.composite(overlayImage, 0, 0, self.topoOverlay.props.width, self.topoOverlay.props.height, 0, 0, 1.0, 1.0, INTERPOLATION_TYPE, 255)
#
#		if self.wTree.get_widget("countycheck").get_active():
#			self.countyOverlay = self.getOverlay("county")
#			self.countyOverlay.composite(overlayImage, 0, 0, self.countyOverlay.props.width, self.countyOverlay.props.height, 0, 0, 1.0, 1.0, INTERPOLATION_TYPE, 255)
#
#		if self.wTree.get_widget("highwaycheck").get_active():
#			self.highwayOverlay = self.getOverlay("highway")
#			self.highwayOverlay.composite(overlayImage, 0, 0, self.highwayOverlay.props.width, self.highwayOverlay.props.height, 0, 0, 1.0, 1.0, INTERPOLATION_TYPE, 255)
#
#		if self.wTree.get_widget("citycheck").get_active():
#			self.cityOverlay = self.getOverlay("city")
#			self.cityOverlay.composite(overlayImage, 0, 0, self.cityOverlay.props.width, self.cityOverlay.props.height, 0, 0, 1.0, 1.0, INTERPOLATION_TYPE, 255)
#
#		self.overlayImage = overlayImage.copy()
#		#self.cacheRadarComposites()
#
#
#	def getRadarFiles(self):
#		self.wTree.get_widget("label3").set_text('Analyzing radar image database...')
#		while gtk.events_pending():
#			gtk.main_iteration(False)
#
#		self.radarImageDB = radarImageDB(self.stID, self.radarType, radarPath)
#		if self.radarImageDB.scanRadarDirectory() > 10:
#			self.radarImageDB.computeRadarEpochs()
#			self.displayStormSelectDialog()
#
#		self.radarXStart = self.radarYStart = 0
#		self.radarWidth = self.radarXEnd = RADAR_WIDTH #self.topoOverlay.props.width
#		self.radarHeight = self.radarYEnd = RADAR_HEIGHT #self.topoOverlay.props.height
#
#		self.dataInit = True
#		self.renderOverlays()
#
#
#	def showLatest(self):
#		if not self.dataInit:
#			return False
#		if not self.playing:
#			self.looppos = 0
#			self.displayRadarImage()
#			#if len(self.radarFiles):
#				#self.radarview.set_from_file(os.path.expanduser(os.path.normpath(radarPath+"/"+self.radarFiles[1])))
#
#
#	def displayRadarImage(self):
#		self.radarVisible = True
#		radarOverlay = self.overlayImage.copy()
#		try:
#			radarFile = self.selectedEpoch.fileList[self.looppos]
#			#radarFile = self.radarImageDB.getFile[self.looppos]
#			#radarFile = self.radarFiles[self.looppos]
#		except:
#				#radarFile = self.selectedEpoch.fileList[self.looppos]
#			print "Error loading radar file %d/%d" % (self.looppos, self.frameLimit)
#			return False
#
#		try:
#			#radarCache = self.radarImages[self.looppos]
#			radarImage = gtk.gdk.pixbuf_new_from_file(os.path.expanduser(os.path.normpath(radarPath + os.sep + radarFile)))
#		except:
#			print "unable to find image file %s (%d)" % (radarPath + os.sep + radarFile, self.looppos)
#			return False
#
#		radarImage.composite(radarOverlay, 0, 0, RADAR_WIDTH, RADAR_HEIGHT, 0, 0, 1, 1, gtk.gdk.INTERP_BILINEAR, self.opacity)
#
#		# Resize and resample our image
#		try:
#			scalePixBuf = radarOverlay.subpixbuf(self.radarXStart, self.radarYStart, self.radarXEnd-self.radarXStart, self.radarYEnd-self.radarYStart)
#		except:
#			print 'scale fail!'
#			print 'radarXStart,radarYStart = (%d, %d)' % (self.radarXStart, self.radarYStart)
#			print 'radarXEnd,radarYEnd = (%d, %d)' % (self.radarXEnd, self.radarYEnd)
#
#		#resizePixBuf = scalePixBuf.scale_simple(self.radarWidth, self.radarHeight, gtk.gdk.INTERP_NEAREST)
#		resizePixBuf = scalePixBuf.scale_simple(self.radarWidth, self.radarHeight, INTERPOLATION_TYPE)
#		#resizePixBuf = scalePixBuf.scale_simple(self.radarWidth, self.radarHeight, gtk.gdk.INTERP_HYPER)
#		self.radarview.set_from_pixbuf(resizePixBuf)
#
#		# Filename -> status line
#		status = "%s [%d/%d]" % (self.selectedEpoch.fileList[self.looppos], self.looppos+1, self.frameLimit)
#		self.wTree.get_widget("label3").set_text(status)
#
#		if self.playing:
#			self.looppos = self.looppos + 1
#			if self.looppos >= self.frameLimit:
#				self.looppos = 0
#				gobject.timeout_add(1500, self.delayChanged)		# Pause for a bit before re-looping
#				return False										# Return false to stop timer from repeating this function
#
#		return self.playing
#
#
#	def goFirstFrame(self=None, widget=None, data=None):
#		if self.dataInit:
#			print 'first frame...'
#			self.looppos = 0
#			self.displayRadarImage()
#
#
#	def goPreviousFrame(self=None, widget=None, data=None):
#		if self.dataInit:
#			print 'rewind frame...'
#			self.looppos -= 1
#			if self.looppos < 0:
#				self.looppos = self.frameLimit-1
#			self.displayRadarImage()
#
#
#	def startAnimation(self=None, widget=None, data=None):
#		if self.dataInit:
#			if not self.playing:
#				print 'Starting animation playback...'
#				timerLen = int(self.wTree.get_widget("delayhscale").get_value())
#				print "using timerlen " + str(timerLen)
#				self.playing = True
#				try:
#					print 'adding timer...'
#					if self.displayTimer:
#						gobject.source_remove(self.displayTimer)
#					self.displayTimer = 0
#					self.displayTimer = gobject.timeout_add(timerLen, self.displayRadarImage)
#				except:
#					a = 0
#			else:
#				return False
#		else:
#			return False
#
#
#	def loopToggled(self=None, widget=None, data=None):
#		self.startAnimation()
#		#if self.dataInit and self.wTree.get_widget("loopbutton").get_active():
#			#print 'Starting looping...'
#			#timerLen = int(self.wTree.get_widget("delayhscale").get_value())
#			#print "using timerlen " + str(timerLen)
#			##timerLen = 500
#			##self.initLoop()
#			#self.looppos = 0
#			#self.playing = True
#			#try:
#				#print 'adding timer...'
#				#if self.displayTimer:
#					#gobject.source_remove(self.displayTimer)
#				#self.displayTimer = 0
#				#self.displayTimer = gobject.timeout_add(timerLen, self.displayRadarImage)
#			#except:
#				#a = 0
#		#else:
#			#return False
#			##self.firstLoop = 1
#
#
#	def stopAnimation(self=None, widget=None, data=None):
#		self.playing = False
#		gobject.source_remove(self.displayTimer)
#
#
#	def goNextFrame(self=None, widget=None, data=None):
#		if self.dataInit:
#			print 'advance frame...'
#			self.looppos += 1
#			if self.looppos == self.frameLimit:
#				self.looppos = 0
#			self.displayRadarImage()
#
#
#	def goLastFrame(self=None, widget=None, data=None):
#		if self.dataInit:
#			print 'last frame...'
#			self.looppos = self.frameLimit-1
#			self.displayRadarImage()
#
#
#	#def initLoop(self):
#		#self.wTree.get_widget("label3").set_text('Retrieving latest loop radar images...')
#		#while gtk.events_pending():
#			#gtk.main_iteration(False)
#		#self.wTree.get_widget("label3").set_text('Latest radar image retrieved at '+time.strftime('%a %b %d, %Y - %H:%M:%S %Z', time.localtime())+'...')
#
#		#newRS = str(self.wTree.get_widget("radarstation").get_active_text())
#		#stID = self.getStationID(newRS)[1:]
#		#rdType = str(self.wTree.get_widget("radartype").get_active_text())
#		#curRadar = radarTypes[rdType]
#
#
#
#	def imageClicked(self, widget, event):
#		if not self.radarVisible: return
#
#        # Get our cursor coordinates
#		xCursor, yCursor, state = event.window.get_pointer()
#		if xCursor > 5: xCursor = xCursor - 5
#
#        # Calculate relative position
#		aspectRatio = float(self.radarWidth) / self.radarHeight
#		xCursorRatio = float(xCursor) / self.radarWidth
#		yCursorRatio = float(yCursor) / self.radarHeight
#
#		if xCursorRatio > 1.0: xCursorRatio = 1.0
#		if yCursorRatio > 1.0: yCursorRatio = 1.0
#
#		radarXSize = self.radarXEnd - self.radarXStart
#		radarYSize = self.radarYEnd - self.radarYStart
#
#		avgSize = (radarXSize + radarYSize) / 2
#		radarXSize = avgSize * aspectRatio          # Preserve our aspect ratio
#		radarYSize = avgSize
#
#		xRadar = int(xCursorRatio * radarXSize) + self.radarXStart
#		yRadar = int(yCursorRatio * radarYSize) + self.radarYStart
#
#		print '\nbutton ' + str(event.button) + ' clicked!'
#		print 'preparing to pan/zoom...'
#		print '(xCursor,yCursor) = (%d,%d)' % (xCursor, yCursor)
#		print '(xCursorRatio,yCursorRatio) = (%f,%f)' % (xCursorRatio, yCursorRatio)
#		print '(xRadar,yRadar) = (%d,%d)' % (xRadar,yRadar)
#		print '(radarXSize,radarYSize) = (%d,%d)' % (radarXSize,radarYSize)
#
#		newRadarXSize = 0
#		if event.button == 1:
#			if radarXSize < 50 or radarYSize < 50:
#				return False
#			newRadarXSize = int(radarXSize * 0.66)
#			newRadarYSize = int(radarYSize * 0.66)
#		if event.button == 3:
#			newRadarXSize = int(radarXSize * 2)
#			newRadarYSize = int(radarYSize * 2)
#		if newRadarXSize > 0:
#			newRadarXStart = xRadar - int(newRadarXSize * xCursorRatio)
#			newRadarYStart = yRadar - int(newRadarYSize * yCursorRatio)
#			self.radarXStart = newRadarXStart
#			self.radarYStart = newRadarYStart
#			self.radarXEnd = newRadarXStart + newRadarXSize
#			self.radarYEnd = newRadarYStart + newRadarYSize
#
#		if self.radarXStart < 0: self.radarXStart = 0
#		if self.radarYStart < 0: self.radarYStart = 0
#		if self.radarXEnd > self.radarWidth:
#			print 'rescaling x...'
#			self.radarXEnd = self.radarWidth
#		if self.radarYEnd > self.radarHeight:
#			print 'rescaling y...'
#			self.radarYEnd = self.radarHeight
#
#		print 'new:'
#		print '\t(radarXStart,radarYStart) = (%d,%d)' % (self.radarXStart, self.radarYStart)
#		print '\t(radarXEnd,radarYEnd) = (%d,%d)' % (self.radarXEnd, self.radarYEnd)
#		print '\t(radarXSize,radarYSize) = (%d,%d)' % (self.radarXEnd-self.radarXStart, self.radarYEnd-self.radarYStart)
#
#		return False
#
#
#	#def cacheRadarComposites(self):
#		#self.radarImages = []
#		#if not self.overlayImageCopy: return
#		#i = 0
#		#for image in self.radarFiles:
#			#if i > self.frameLimit: return
#			#print 'caching image: (' + str(i) + ") " + image
#			#radarImage = gtk.gdk.pixbuf_new_from_file(os.path.expanduser(os.path.normpath(radarPath + os.sep + image)))
#			#overlay = self.overlayImageCopy.copy()
#			#radarImage.composite(overlay, 0, 0, self.topoOverlay.props.width, self.topoOverlay.props.height, 0, 0, 1.0, 1.0, gtk.gdk.INTERP_HYPER, 255)
#			#self.radarImages.append(overlay)
#			#i = i + 1
#
#
#	def displayStormSelectDialog(self=None, data=None):
#		self.dialog = stormSelectDialog(self.gladefile, self.radarImageDB)
#		self.selectedEpoch = self.dialog.run()
#
#		if self.selectedEpoch != 0:
#			self.frameLimit = len(self.selectedEpoch.fileList)
#			print "Using the following files in loop:"
#			for file in self.selectedEpoch.fileList:
#				print file,
#
#		#year, month, day = calendar.get_date()
#		#month = month + 1		# Months start counting at zero for some reason
#		#print self.popupDialog.output
#		#print "Y-m-d: " + str(year) + "-" + str(month) + "-" + str(day)
#		#self.popupDialog.destroy()
#


class radarWatch:

	def __init__(self):
		parser = OptionParser()		
		parser.add_option("-r", "--radarpath", dest=radarPath,
			help="path to radar files")
		(options, args) = parser.parse_args()

		self.context = zmq.Context()

		# Socket to send messages on
		self.processor = self.context.socket(zmq.REQ)
		self.processor.connect("tcp://localhost:5555")

		# Socket with direct access to the sink: used to syncronize start of batch
#		self.sink = self.context.socket(zmq.PUSH)
#		self.sink.connect("tcp://localhost:5558")

		self.archive = nexradGifArchive(fsPath=radarPath, processor=self.processor)
		self.archive.scanRadarDirectory()
		self.archive.processImages()


class epoch:

	def __init__(self, start):
		self.start = start
		self.end = 0
		self.fileList = []

	def setEnd(self, end):
		self.end = end

	def addFile(self, file):
		self.fileList.append(file)

	def getStart(self):
		return self.start

	def getEnd(self):
		return self.end

	def getStartArr(self):
		return [self.start.year, self.start.month, self.start.day, self.start.hour, self.start.minute]

	def getEndArr(self):
		return [self.end.year, self.end.month, self.end.day, self.end.hour, self.end.minute]

	def getFile(self, index):
		return self.fileList[index]

	def getLength(self):
		timedelta = abs(end - start)

	def searchFiles(self, searchFile):
		if searchFile in self.fileList:
			return True

	def getFileIndex(self, searchFile):
		for index in range(len(self.fileList)):
			if self.fileList[index] == searchFile:
				return index


#class radarImagePath:
#	def __init__(self):
#		return


#import cPyNexrad

class nexradGif:
	def __init__(self, filename, timestamp):
		self.filename = filename
		self.timestamp = timestamp
		self.unixepoch = (timestamp - datetime(1970,1,1)).total_seconds()
	
	def getFilename(self):
		return self.filename

	def getTimestamp(self):
		return self.timestamp

	def getUnixEpoch(self):
		return self.unixepoch

	def process(self):
		# Hand off to our C module to process
		#result = cPyNexrad.process(self)
		return True


class nexradGifArchive: # extends radarImagePath:

	def __init__(self, processor, stationID='DIX', NOAAProduct='NCR', fsPath="."):
		self.radarType = {}
		self.stationID = stationID
		self.radarType['NOAAProd'] = NOAAProduct;
		self.dataPath = fsPath
		self.processor = processor 


	# Populate radarFiles with files in radar directory matching regex
	def scanRadarDirectory(self):
		radarFilenameRE = '%s_([0-9]{4})([0-9]{2})([0-9]{2})_([0-9]{2})([0-9]{2})_%s.gif(?i)' % (self.stationID, self.radarType['NOAAProd'])
		#print 'searching "' + self.dataPath + '" for station id: "' + str(self.stationID) + '" and radar type: "' + str(self.radarType['NOAAProd']) + '"'
		print 'reading in files...'

		totalCount = 0
		self.radarFiles = []
		for filename in os.listdir(self.dataPath):
			totalCount += 1
			match = re.match(radarFilenameRE, filename)
			if not match: continue

			year = int(match.group(1))
			month = int(match.group(2))
			day = int(match.group(3))
			hour = int(match.group(4))
			minute = int(match.group(5))

			#newImage = radarImage(
			#print '%d %d %d %d %d' % (year, month, day, hour, minute)

			# Do we compute epoch here too (UNIX-timestamp)?
			# Definitely should be sorting on epoch (seconds since 1/1/70)
			# especially if incorporating multiple stations
			timestamp = datetime(year, month, day, hour, minute)
			#timePeriod = epoch(timestamp)

			radarImage = nexradGif(filename=filename, timestamp=timestamp)
			self.radarFiles.append(radarImage)

		# This is crucial for computing epochs correctly
		# Sort by timestamp! (epoch)
		# TODO: this will break if using more then one station in the same image pool
		# Should sort by timestamp, not filename
		#self.radarFiles = sorted(self.radarFiles, key=lambda item: item.getFilename())
		self.radarFiles = sorted(self.radarFiles, key=lambda item: item.getUnixEpoch())

		print "done, %d/%d found" % (len(self.radarFiles), totalCount)
		return len(self.radarFiles)


	def processImages(self):
		#sink.send('0')

		timestamp = 0
		fileCheckedCount = 0
		radarFileCount = len(self.radarFiles)
		for image in self.radarFiles:

			if not timestamp:
				timestamp = image.getTimestamp()
				print timestamp
				#timePeriod = epoch(timestamp)

			lastTimestamp = timestamp
			timestamp = image.getTimestamp()
			timedelta = abs(timestamp - lastTimestamp)
#			import pdb; pdb.set_trace()

#			epochstamp = time.mktime(timestamp.timetuple())
#			print >> epochlist, "%d, %d, %d, %d, %d, %d" % (year, month, day, hour, minute, epochstamp)

			fileCheckedCount += 1
			#epoch = timedelta(timestamp)
			print "\n\nradar image : %s [%d/%d]" % (image.getFilename(), fileCheckedCount, radarFileCount)
			print "timestamp : %s, epoch = %d..." % (timestamp.ctime(), image.getUnixEpoch()),

			self.processor.send(image.getFilename())
			ret = self.processor.recv()
			if ret == 'ACK':
				print "OK"
			else:
				print ret
			#break


	# Parse timstamps on radarFiles and compute epochs (sort and search for gaps)
	# TODO: Should be changed into a post-analysis cleanup/compaction routine
	#def computeRadarEpochs(self):
	def groupRadarTimeSeries(self):
		#epochlist = open('epochs.csv', 'w')

		return True

		fileCheckedCount = 0
		radarFileCount = len(self.radarFiles)
		for image in self.radarFiles:

			if not timestamp:
				timestamp = image['ts']
				timePeriod = epoch(timestamp)

			lastTimestamp = timestamp
			timestamp = image['ts']
			timedelta = abs(timestamp - lastTimestamp)

#			epochstamp = time.mktime(timestamp.timetuple())
#			print >> epochlist, "%d, %d, %d, %d, %d, %d" % (year, month, day, hour, minute, epochstamp)

			fileCheckedCount += 1
			epoch = timedelta(timestamp)
			print "\n\nradar image : %d/%d\n" % (fileCheckedCount, radarFileCount)
			print "source image : %s\n" % image['name']
			print "timestamp : %s, epoch = %d\n" % (timestamp.ctime(), (timestamp - datetime(1970,1,1).total_seconds()))

			# Only usec/seconds/days are stored internally in timedelta objects
			if timedelta.seconds > 4000 or timedelta.days > 0 or counter == radarFileCount:
				if counter == radarFileCount: print '\tcounter hit %d limit' % (radarFileCount)
				print '\tsignificant non-linearality found: %d s (%d d)' % (timedelta.seconds, timedelta.days)
				timePeriod.setEnd(lastTimestamp)
				print '\tadding epoch: ' + str(timePeriod.getStartArr()) + " - " + str(timePeriod.getEndArr())
				self.epochs.append(timePeriod)
				timePeriod = epoch(timestamp)

			timePeriod.addFile(file)

		epochlist.close()




##class radarImageDB:
#
#	def __init__(self, locID, fsPath):
#		self.locationID = locID 
#		self.dataPath = fsPath
#		self.radarFileDB = []
#		self.epochs = []
#		self.epochIndex = 0
#
#		#self.refreshImageList()
#		#self.scanRadarDirectory()
#
#
#	def initImageDB(self):
#		print 'initImageDB()...start'
#		self.scanRadarDirectory()
#		self.computeRadarEpochs()
#		print 'initImageDB()...end'
#
#
#
#	#def queryMonth(self, year, month):
#		#dayList = []
#		#epochList = {}
#
#		#print "================  IN queryMonth()  ==================="
#
#		#for epoch in self.epochs:
#			#epochStart = epoch.getStart()
#			##print 'checking %s...' % (epoch.getStartArr())
#
#			#if epochStart.year == year and epochStart.month == month:
#				#dayStart = epoch.getStart().day
#				#dayEnd = epoch.getEnd().day
#				#print 'start %d, end %d, ' % (dayStart, dayEnd)
#
#				#if dayStart == dayEnd: dayEnd += 1		# range(a,b)) only goes from a to b-1
#				#for day in range(dayStart, dayEnd):
#					
#					#if not dayList.count(day):
#						#print '\tadding day %d' % (day)
#						
#						## TODO: add epoch index here too, use dictionary instead of list
#						#dayList.append(day)
#
#		#return dayList
#
#
#	# Given epoch and index return file 
#	def getRadarFile(self, index):
#		if not self.epochIndex:
#			print 'Error : epoch index not selected before calling getRadarFile()!'
#			return
#
#		epoch = self.epochs[self.epochIndex]
#		file = epoch.getFile(index)
#		return file
#
#
#
#class stormSelect:
#
#	def __init__(self, imageDB=None):
#		self.imageDB = imageDB
#
#
#	def getDB(self):
#		return self.imageDB
#		## Create the dialog, show it, and store the results
#		#stormSelectDlg = stormSelectDialog();
#		#result,newWine = stormSelectDlg.run()
#
#		#if (result == gtk.RESPONSE_OK):
#		#"""The user clicked Ok, so let's add this
#		#wine to the wine list"""
#		#self.wineList.append(newWine.getList())
#
#
#
#class stormSelectDialog:
#
#	def __init__(self, gladefile="", imageDB=""):
#		self.gladefile = gladefile
#
#		self.cDate = 0
#		self.cFile = 1
#		self.cEpoch = 2
#
#		self.sDate = "Date/Time"
#		self.sFile = "Filename"
#
#		# setup the class that we will return
#		self.data = stormSelect(imageDB)
#
#		# Load our widget tree
#		self.wTree = gtk.glade.XML(self.gladefile, "stormselectdialog")
#
#		# Connect signal callbacks for the calendar
#		self.calendar = self.wTree.get_widget("calendar1")
#		self.calendar.connect("month_changed", self.renderCalendar)
#		self.calendar.connect("day_selected", self.calDayChanged)
#		self.calendar.connect("prev_year", self.calYearChanged)
#		self.calendar.connect("next_year", self.calYearChanged)
#
#		today = datetime.today()
#		self.calendar.select_month(today.month-1, today.year)
#		self.calendar.select_day(today.day)
#
#		#sig = { "day_selected" : self.calDayChanged, \
#				#"month_changed" : self.renderCalendar, \
#				#"prev_year" : self.calYearChanged, \
#				#"next_year" : self.calYearChanged, \
#				##"on_allstormsradio_group_changed" : self.newTimer, \
#				##"on_30minbutton_toggled" : self.newTimer, \
#				##"on_60minbutton_toggled" : self.newTimer, \
#				##"on_stormbutton_clicked" : self.displayStormSelectDialog, \
#				##"on_delay_value_changed" : self.delayChanged, \
#				##"on_loopbutton_toggled" : self.loopToggled, \
#				##"on_overlay_toggled" : self.renderOverlays, \
#				##"on_radarimage1_release_event" : self.imageClicked, \
#				##"on_radarimage1_buttonpress_event" : self.imageClicked
#		#}
#		#self.wTree.signal_autoconnect(sig)
#
#		# Setup the treeView
#		self.stormView = self.wTree.get_widget("stormview")
#
#		# Add all of the List Columns to the stormView (except the epoch object)
#		self.addStormListColumn(self.sDate, self.cDate, 100)
#		self.addStormListColumn(self.sFile, self.cFile, 200)
#
#		# Create the listStore Model to use with the stormView
#		self.stormList = gtk.TreeStore(str, str, gobject.TYPE_PYOBJECT)
#
#		# Attatch the model to the treeView
#		self.stormView.set_model(self.stormList)
#
#		# Set treeview data
#		self.treeViewFullData = True
#
#		# Populate our calendar with data
#		self.renderCalendar()
#
#
#	# Add a column to the treeview widget
#	def addStormListColumn(self, title, columnId, minwidth):
#		column = gtk.TreeViewColumn(title, gtk.CellRendererText(), text=columnId)
#		column.set_resizable(True)
#		column.set_min_width(minwidth)
#		column.set_sort_column_id(columnId)
#		self.stormView.append_column(column)
#
#
#	def run(self):
#		# Get the actual dialog widget
#		self.dialog = self.wTree.get_widget("stormselectdialog")
#
#		# Connect other events
#		radio = self.wTree.get_widget("allstormsradio")
#		radio.connect("toggled", self.stormScopeRadioChanged)
#		#radio2 = self.wTree.get_widget("selecteddaystormsradio")
#
#		# Prepare our dialog
#		widget = self.wTree.get_widget("allstormsradio")
#		widget.set_active(1)
#
#		# run the dialog and store the response
#		self.result = self.dialog.run()
#
#		# get the date
#		year, month, day = self.calendar.get_date()
#		month = month + 1		# Months start counting at zero for some reason
#		print 'dialog returned %d, date: %s' % (self.result, self.dateFormat(year, month, day))
#
#		# abort if cancel
#		if self.result == 2:
#			self.dialog.destroy()
#			return
#
#		# get the treeview selection
#		self.stormSelection = self.stormView.get_selection()
#		(model, iter) = self.stormSelection.get_selected()
#
#		epoch = 0
#		if (iter):
#			epoch = self.stormList.get_value(iter, self.cEpoch)
#
#		print 'dialog returned epoch %s\n' % (epoch)
#
#		# we are done with the dialog, destroy it
#		self.dialog.destroy()
#
#		# return the epoch
#		return epoch
#
#
#	def dateFormat(self, year, month, day):
#		return '%d/%d/%s' % (month, day, str(year)[2:4])
#
#
#	# Return timestamp from a filename
#	def dateTimeStamp(self, filename):
#		radarFullRE = '_([0-9]{4})([0-9]{2})([0-9]{2})_([0-9]{2})([0-9]{2})_\w*(?i)'
#		match = re.search(radarFullRE, filename)
#		year = int(match.group(1))
#		month = int(match.group(2))
#		day = int(match.group(3))
#		hour = int(match.group(4))
#		minute = int(match.group(5))
#		timestamp = datetime(year, month, day, hour, minute)
#		return timestamp
#
#
#	def calDayChanged(self, widget):
#		year, month, day = widget.get_date()
#		month += 1										# Months start counting at zero for some reason
#
#		#iter = self.stormList.append(None, [date, None])
#		#self.stormList.append(iter, [date, '04:20'])
#		#self.stormList.append(['test'])
#		#self.stormList.append('test')
#		#self.stormView.set_model(self.stormList)
#		if not self.treeViewFullData:
#			self.renderCalendar()
#
#		print "date changed, new: " + self.dateFormat(year, month, day)
#		return
#
#
#	def calYearChanged(self, widget):
#		self.renderCalendar(widget)
#
#
#	# Switch between populating treeview widget with all month's data or only selected day
#	def stormScopeRadioChanged(self, widget, data=None):
#		if self.wTree.get_widget("allstormsradio").get_active():
#			self.treeViewFullData = True
#		if self.wTree.get_widget("selecteddaystormsradio").get_active():
#			self.treeViewFullData = False
#		self.renderCalendar()
#
#
#	# Redraw our calendar widget (clear/reset and populate)
#	def renderCalendar(self, widget=None):
#		widget = self.wTree.get_widget("calendar1")
#
#		#if self.stormList:
#		self.stormList.clear()
#		self.calendar.clear_marks()
#
#		year, month, day = widget.get_date()
#		month += 1										# Months start counting at zero for some reason
#		date = self.dateFormat(year, month, day)
#		#print date
#		self.populateCalendar(year, month, day)
#
#
#	# Fill our calendar in with data
#	def populateCalendar(self, year, month, ourDay=None):
#		db = self.data.getDB()
#
#		for epoch in db.epochs:
#			epochStart = epoch.getStart()
#
#			if epochStart.year == year and epochStart.month == month:
#				dayStart = epoch.getStart().day
#				dayEnd = epoch.getEnd().day
#				#print 'start %d, end %d' % (dayStart, dayEnd)
#
#				if dayStart == dayEnd: dayEnd += 1		# range(a,b)) only goes from a to b-1
#				for day in range(dayStart, dayEnd):
#					#print '\tadding day %d' % (day)
#					date = self.dateFormat(year, month, day)
#
#					# Mark the day on the calendar
#					self.calendar.mark_day(day)
#
#					# Check our "full data view" flag and display all days or just if same selected day
#					if self.treeViewFullData or (not self.treeViewFullData and ourDay == day):
#						fileList = []
#						for file in epoch.fileList:
#							datetime = self.dateTimeStamp(file)
#							time = datetime.time()
#							timeStr = time.strftime("%H:%M")
#							if file == epoch.fileList[0]:
#								epochStart = datetime
#							if file == epoch.fileList[len(epoch.fileList)-1]:
#								epochEnd = datetime
#							fileList.append([timeStr, file, epoch])
#
#						timedelta = abs(epochStart - epochEnd)
#						print "%s - %s (length: %s)" % (epochStart, epochEnd, timedelta)
#
#						title = "(%d files, %s length)" % (len(epoch.fileList), timedelta)
#						iter = self.stormList.append(None, [date, title, epoch])
#						for file in fileList:
#							self.stormList.append(iter, file)
#


# Simple Gtk main()

#instance = radarImagePathGif() 
instance = radarWatch()
#instance = nexradGifArchive()
#app=radarWatch()
#gtk.main()

# The End



		#ftp = ftplib.FTP('tgftp.nws.noaa.gov')
		#ftp.login()
		#ftp.cwd('SL.us008001/DF.gif/DC.radar/DS.'+curRadar+'/SI.'+stID+'/')
		#nlist = ftp.nlst()

		#print 'Searching for station ' + str(stID) + ', type ' + str(curRadar)
		#for file in os.listdir(radarPath):
			#if re.match("DIX_[0-9]{8}_[0-9]{4}_"+curRadar+".gif", file) and len(self.radarFiles) <= self.frameLimit:
				#self.radarFiles.append(file)

		#self.radarFiles.sort()
		#print self.radarFiles
		#return

		#for x in nlist:
			#if x.find("dd.") != -1:
				#loopedImages.insert(0, x)
		#loopedImages.sort()
		#y = len(self.radarFiles) - 1
		#self.curframe = 10
		#self.lastfile = self.radarFiles[y]
		#while y >= 0 and self.curframe > 0:
			#try:
				##ftp.retrbinary('RETR '+loopedImages[y], open(os.path.expanduser(os.path.normpath('./images/fr'+str(self.curframe)+'.gif')), 'wb').write)
				#z = (10 - self.curframe) + 1
				#self.wTree.get_widget("label3").set_text('Retrieving loop radar image ('+str(z)+'/10)...')
				#while gtk.events_pending():
					#gtk.main_iteration(False)
				##print "got image "+str(self.curframe)+" "+loopedImages[y]
			#except:
				##print 'ERROR loading: http://weather.gov/radar/images/DS.'+curRadar+'/SI.'+stID+'/'+loopedImages[y]+"."
				#s#hutil.copy(os.path.expanduser(os.path.normpath('./images/radarWatch.gif')),os.path.expanduser(os.path.normpath('./images/fr'+str(self.curframe)+'.gif')))
			#if self.firstLoop == 1:
				##x = 9
				##while x > 0:
					##shutil.copy(os.path.expanduser(os.path.normpath('./images/fr'+str(self.curframe)+'.gif')),os.path.expanduser(os.path.normpath('./images/fr'+str(x)+'.gif')))
					##x = x - 1
				#x = 0
			#self.firstLoop = 0
			#time.sleep(1)
			#self.curframe = self.curframe - 1
			#y = y - 1



		#ftp = ftplib.FTP('tgftp.nws.noaa.gov')
		#ftp.login()
		#ftp.cwd('SL.us008001/DF.gif/DC.radar/DS.'+curRadar+'/SI.'+stID+'/')
		#nlist = ftp.nlst()
		#for x in nlist:
			#if x.find("dd.") != -1:
				#loopedImages.insert(0, x)
		#loopedImages.sort()
		##print "found "+str(len(loopedImages))+" images"
		#y = len(loopedImages) - 1

		#if loopedImages[(y-1)] != self.lastfile and loopedImages[y] != self.lastfile and isLooping:
			#while y >= 0 and loopedImages[y] != self.lastfile:
				#ftp.retrbinary('RETR '+loopedImages[y], open(os.path.expanduser(os.path.normpath('~/.pyradar/images/fr'+str(self.curframe)+'.gif')), 'wb').write)
				#time.sleep(1)
				#self.curframe = self.curframe + 1
				#if self.curframe > 10:
					#self.curframe = 1
				#y = y - 1
		#else:
			#ftp.retrbinary('RETR '+loopedImages[y], open(os.path.expanduser(os.path.normpath('~/.pyradar/images/latest.gif')), 'wb').write)
			#self.wTree.get_widget("label3").set_text('Latest radar image retrieved at '+time.strftime('%a %b %d, %Y - %H:%M:%S', time.gmtime())+' UTC...')
			#time.sleep(1)

			#if isLooping and loopedImages[y] != self.lastfile:
				#loopfile = os.path.expanduser(os.path.normpath('~/.pyradar/images/fr'+str(self.curframe)+'.gif'))
				#shutil.copy(os.path.expanduser(os.path.normpath('~/.pyradar/images/latest.gif')),loopfile)
				#self.curframe = self.curframe + 1
				#if self.curframe > 10:
					#self.curframe = 1
				#self.lastfile = loopedImages[y]
		##print "updated loop"
		#ftp.quit()
