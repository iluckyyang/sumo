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
netedit.setZoom("25", "25", "25")

# go to additional mode
netedit.additionalMode()

# select E3
netedit.changeAdditional("e3Detector")

# create E3
netedit.leftClick(referencePosition, 250, 400)

# create second E3
netedit.leftClick(referencePosition, 350, 400)

# select entry detector
netedit.changeAdditional("detEntry")

# Create Entry detector E3
netedit.leftClick(referencePosition, 250, 400)
netedit.leftClick(referencePosition, 200, 200)

# select exit detector and create in the second E3
netedit.changeAdditional("detExit")
netedit.leftClick(referencePosition, 350, 400)
netedit.leftClick(referencePosition, 400, 200)

# go to inspect mode
netedit.inspectMode()

# inspect Entry
netedit.leftClick(referencePosition, 200, 200)

# Change parameter 0 with a non valid value (dummy Lane)
netedit.modifyAttribute(0, "dummyLane")

# Change parameter 0 with a non valid value (Empty lane)
netedit.modifyAttribute(0, "")

# Change parameter 0 with a valid value (other lane)
netedit.modifyAttribute(0, "gneE3_1")

# Change parameter 1 with a non valid value (dummy position X)
netedit.modifyAttribute(1, "dummy position")

# Change parameter 1 with a non valid value (empty)
netedit.modifyAttribute(1, "")

# Change parameter 1 with a valid value (different position X)
netedit.modifyAttribute(1, "25")

# Change boolean parameter 2
netedit.modifyBoolAttribute(2)

# Change Netedit parameter 1 with a non valid value (Invalid E3 ID)
netedit.modifyAttribute(6, "invalidE3")

# Change Netedit parameter 2 with a non valid value (Invalid E3 ID)
netedit.modifyAttribute(6, "e3Detector_1")

# Change Netedit boolean parameter 3 (block)
netedit.modifyBoolAttribute(7)

# Check undos and redos
netedit.undo(referencePosition, 9)
netedit.redo(referencePosition, 9)

# save additionals
netedit.saveAdditionals()

# save network
netedit.saveNetwork()

# quit netedit
netedit.quit(neteditProcess)
