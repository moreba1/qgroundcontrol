/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#include "EditPositionDialogController.h"

const char*  EditPositionDialogController::_latitudeFactName =      "Latitude";
const char*  EditPositionDialogController::_longitudeFactName =     "Longitude";
const char*  EditPositionDialogController::_zoneFactName =          "Zone";
const char*  EditPositionDialogController::_hemisphereFactName =    "Hemisphere";
const char*  EditPositionDialogController::_eastingFactName =       "Easting";
const char*  EditPositionDialogController::_northingFactName =      "Northing";

QMap<QString, FactMetaData*> EditPositionDialogController::_metaDataMap;

EditPositionDialogController::EditPositionDialogController(void)
    : _latitudeFact     (0, _latitudeFactName,      FactMetaData::valueTypeDouble)
    , _longitudeFact    (0, _longitudeFactName,     FactMetaData::valueTypeDouble)
    , _zoneFact         (0, _zoneFactName,          FactMetaData::valueTypeUint8)
    , _hemisphereFact   (0, _hemisphereFactName,    FactMetaData::valueTypeBool)
    , _eastingFact      (0, _eastingFactName,       FactMetaData::valueTypeDouble)
    , _northingFact     (0, _northingFactName,      FactMetaData::valueTypeDouble)
{
    if (_metaDataMap.isEmpty()) {
        _metaDataMap = FactMetaData::createMapFromJsonFile(QStringLiteral(":/json/EditPositionDialog.FactMetaData.json"), NULL /* QObject parent */);
    }

    _latitudeFact.setMetaData   (_metaDataMap[_latitudeFactName]);
    _longitudeFact.setMetaData  (_metaDataMap[_longitudeFactName]);
    _zoneFact.setMetaData       (_metaDataMap[_zoneFactName]);
    _hemisphereFact.setMetaData (_metaDataMap[_hemisphereFactName]);
    _eastingFact.setMetaData    (_metaDataMap[_eastingFactName]);
    _northingFact.setMetaData   (_metaDataMap[_northingFactName]);
}

void EditPositionDialogController::setCoordinate(QGeoCoordinate coordinate)
{
    if (coordinate != _coordinate) {
        _coordinate = coordinate;
        emit coordinateChanged(coordinate);
    }
}

void EditPositionDialogController::initValues(void)
{
    _latitudeFact.setRawValue(_coordinate.latitude());
    _longitudeFact.setRawValue(_coordinate.longitude());
}

void EditPositionDialogController::setFromGeo(void)
{
    _coordinate.setLatitude(_latitudeFact.rawValue().toDouble());
    _coordinate.setLongitude(_longitudeFact.rawValue().toDouble());
    emit coordinateChanged(_coordinate);
}

void EditPositionDialogController::setFromUTM(void)
{

}
