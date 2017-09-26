/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#pragma once

#include "UnitTest.h"
#include "MultiSignalSpy.h"
#include "FixedWingLandingComplexItem.h"

class FixedWingLandingPatternTest : public UnitTest
{
    Q_OBJECT
    
public:
    FixedWingLandingPatternTest(void);

protected:
    void init(void) final;
    void cleanup(void) final;
    
private slots:
    void _testDirty(void);

private:
    enum {
        loiterCoordinateChangedIndex = 0,
        loiterTangentCoordinateChangedIndex,
        landingCoordinateChangedIndex,
        landingCoordSetChangedIndex,
        loiterClockwiseChangedIndex,
        loiterAltitudeRelativeChangedIndex,
        landingAltitudeRelativeChangedIndex,
        dirtyChangedIndex,
        maxSignalIndex
    };

    enum {
        loiterCoordinateChangedMask =           1 << loiterCoordinateChangedIndex,
        loiterTangentCoordinateChangedMask =    1 << loiterTangentCoordinateChangedIndex,
        landingCoordinateChangedMask =          1 << landingCoordinateChangedIndex,
        landingCoordSetChangedMask =            1 << landingCoordSetChangedIndex,
        loiterClockwiseChangedMask =            1 << loiterClockwiseChangedIndex,
        loiterAltitudeRelativeChangedMask =     1 << loiterAltitudeRelativeChangedIndex,
        landingAltitudeRelativeChangedMask =    1 << landingAltitudeRelativeChangedIndex,
        dirtyChangedMask =                      1 << dirtyChangedIndex
    };

    static const size_t _cSignals = maxSignalIndex;
    const char*         _rgSignals[_cSignals];

    Vehicle*                        _offlineVehicle;
    MultiSignalSpy*                 _multiSpy;
    FixedWingLandingComplexItem*    _landingItem;
};
