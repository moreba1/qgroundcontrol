/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#include "FixedWingLandingPatternTest.h"
#include "QGCApplication.h"

FixedWingLandingPatternTest::FixedWingLandingPatternTest(void)
    : _offlineVehicle(NULL)
{

}

void FixedWingLandingPatternTest::init(void)
{
    UnitTest::init();

    _rgSignals[loiterCoordinateChangedIndex] =          SIGNAL(loiterCoordinateChanged(QGeoCoordinate));
    _rgSignals[loiterTangentCoordinateChangedIndex] =   SIGNAL(loiterTangentCoordinateChanged(QGeoCoordinate));
    _rgSignals[landingCoordinateChangedIndex] =         SIGNAL(landingCoordinateChanged(QGeoCoordinate));
    _rgSignals[landingCoordSetChangedIndex] =           SIGNAL(landingCoordSetChanged(bool));
    _rgSignals[loiterClockwiseChangedIndex] =           SIGNAL(loiterClockwiseChanged(bool));
    _rgSignals[loiterAltitudeRelativeChangedIndex] =    SIGNAL(loiterAltitudeRelativeChanged(bool));
    _rgSignals[landingAltitudeRelativeChangedIndex] =   SIGNAL(landingAltitudeRelativeChanged(bool));
    _rgSignals[dirtyChangedIndex] =                     SIGNAL(dirtyChanged(bool));

    _offlineVehicle = new Vehicle(MAV_AUTOPILOT_PX4, MAV_TYPE_FIXED_WING, qgcApp()->toolbox()->firmwarePluginManager(), this);
    _landingItem = new FixedWingLandingComplexItem(_offlineVehicle, this);
    _landingItem->setLandingCoordinate(QGeoCoordinate(47.633550640000003, -122.08982199));
    _landingItem->setDirty(false);

    _multiSpy = new MultiSignalSpy();
    Q_CHECK_PTR(_multiSpy);
    QCOMPARE(_multiSpy->init(_landingItem, _rgSignals, _cSignals), true);
}

void FixedWingLandingPatternTest::cleanup(void)
{
    delete _landingItem;
    delete _offlineVehicle;
    delete _multiSpy;
}

void FixedWingLandingPatternTest::_testDirty(void)
{
    QVERIFY(!_landingItem->dirty());
    _landingItem->setDirty(false);
    QVERIFY(!_landingItem->dirty());
    QVERIFY(_multiSpy->checkNoSignals());

    _landingItem->setDirty(true);
    QVERIFY(_landingItem->dirty());
    QVERIFY(_multiSpy->checkOnlySignalByMask(dirtyChangedMask));
    QVERIFY(_multiSpy->pullBoolFromSignalIndex(dirtyChangedIndex));
    _multiSpy->clearAllSignals();

    _landingItem->setDirty(false);
    QVERIFY(!_landingItem->dirty());
    QVERIFY(_multiSpy->checkOnlySignalByMask(dirtyChangedMask));
    QVERIFY(!_multiSpy->pullBoolFromSignalIndex(dirtyChangedIndex));
    _multiSpy->clearAllSignals();

    // These facts should set dirty when changed
    QList<Fact*> rgFacts;
    rgFacts << _landingItem->loiterAltitude() << _landingItem->loiterRadius() << _landingItem->landingAltitude() << _landingItem->landingDistance() << _landingItem->landingHeading();
    foreach(Fact* fact, rgFacts) {
        qDebug() << fact->name();
        QVERIFY(!_landingItem->dirty());
        fact->setRawValue(fact->rawValue().toDouble() + 1);
        QVERIFY(_multiSpy->checkSignalByMask(dirtyChangedMask));
        QVERIFY(_multiSpy->pullBoolFromSignalIndex(dirtyChangedIndex));
        _landingItem->setDirty(false);
        _multiSpy->clearAllSignals();
    }
    rgFacts.clear();
}
