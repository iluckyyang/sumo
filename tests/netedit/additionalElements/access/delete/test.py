#!/usr/bin/env python
# Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
# Copyright (C) 2009-2018 German Aerospace Center (DLR) and others.
# This program and the accompanying materials
# are made available under the terms of the Eclipse Public License v2.0
# which accompanies this distribution, and is available at
# http://www.eclipse.org/legal/epl-v20.html
# SPDX-License-Identifier: EPL-2.0

# @file    test.py
# @author  Pablo Alvarez Lopez
# @date    2016-11-25
# @version $Id$

# import common functions for netedit tests
import os
import sys

testRoot = os.path.join(os.environ.get('SUMO_HOME', '.'), 'tests')
neteditTestRoot = os.path.join(
    os.environ.get('TEXTTEST_HOME', testRoot), 'netedit')
sys.path.append(neteditTestRoot)
import neteditTestFunctions as netedit  # noqa

# Open netedit
neteditProcess, referencePosition = netedit.setupAndStart(neteditTestRoot, ['--gui-testing-debug-gl'])

# apply zoom
netedit.setZoom("25", "20", "25")

# go to additional mode
netedit.additionalMode()

# select BusStop
netedit.changeAdditional("busStop")

# create BusStop with default parameters
netedit.leftClick(referencePosition, 375, 250)

# select Access 
netedit.changeAdditional("access")

# Create Access 
netedit.selectAdditionalChild(7, 0)
netedit.leftClick(referencePosition, 200, 50)

# Create second Access
netedit.selectAdditionalChild(7, 0)
netedit.leftClick(referencePosition, 200, 250)

# go to inspect mode
netedit.deleteMode()

# delete Access
netedit.leftClick(referencePosition, 208, 260)

# undo
netedit.undo(referencePosition, 1)

# go to inspect mode
netedit.deleteMode()

# delete busStop
netedit.leftClick(referencePosition, 420, 295)

# undo
netedit.undo(referencePosition, 1)

# go to inspect mode
netedit.deleteMode()

# delete both acces
netedit.leftClick(referencePosition, 208, 81)
netedit.leftClick(referencePosition, 208, 260)

# Check undo redo
netedit.undo(referencePosition, 5)
netedit.redo(referencePosition, 5)

# save additionals
netedit.saveAdditionals()

# save network
netedit.saveNetwork()

# quit netedit
netedit.quit(neteditProcess)
