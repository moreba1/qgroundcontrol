/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/


#ifndef StructureScanComplexItem_H
#define StructureScanComplexItem_H

#include "ComplexMissionItem.h"
#include "MissionItem.h"
#include "SettingsFact.h"
#include "QGCLoggingCategory.h"
#include "QGCMapPolygon.h"

Q_DECLARE_LOGGING_CATEGORY(StructureScanComplexItemLog)

class StructureScanComplexItem : public ComplexMissionItem
{
    Q_OBJECT

public:
    StructureScanComplexItem(Vehicle* vehicle, QObject* parent = NULL);

    Q_PROPERTY(Fact*            altitude                    READ altitude                       CONSTANT)
    Q_PROPERTY(Fact*            altitudeRelative            READ altitudeRelative               CONSTANT)
    Q_PROPERTY(Fact*            layers                      READ layers                         CONSTANT)
    Q_PROPERTY(Fact*            layerDistance               READ layerDistance                  CONSTANT)
    Q_PROPERTY(Fact*            cameraTriggerDistance       READ cameraTriggerDistance          CONSTANT)
    Q_PROPERTY(int              cameraShots                 READ cameraShots                    NOTIFY cameraShotsChanged)
    Q_PROPERTY(double           timeBetweenShots            READ timeBetweenShots               NOTIFY timeBetweenShotsChanged)
    Q_PROPERTY(double           cameraMinTriggerInterval    MEMBER _cameraMinTriggerInterval    NOTIFY cameraMinTriggerIntervalChanged)
    Q_PROPERTY(QGCMapPolygon*   mapPolygon                  READ mapPolygon                     CONSTANT)

    Fact* altitude              (void) { return &_altitudeFact; }
    Fact* altitudeRelative      (void) { return &_altitudeRelativeFact; }
    Fact* layers                (void) { return &_layersFact; }
    Fact* layerDistance         (void) { return &_layerDistanceFact; }
    Fact* cameraTriggerDistance (void) { return &_cameraTriggerDistanceFact; }

    int             cameraShots     (void) const;
    double          timeBetweenShots(void) const;
    QGCMapPolygon*  mapPolygon      (void) { return &_mapPolygon; }

    // Overrides from ComplexMissionItem

    double          complexDistance     (void) const final { return _scanDistance; }
    int             lastSequenceNumber  (void) const final;
    bool            load                (const QJsonObject& complexObject, int sequenceNumber, QString& errorString) final;
    double          greatestDistanceTo  (const QGeoCoordinate &other) const final;
    QString         mapVisualQML        (void) const final { return QStringLiteral("StructureScanMapVisual.qml"); }

    // Overrides from VisualMissionItem

    bool            dirty                   (void) const final { return _dirty; }
    bool            isSimpleItem            (void) const final { return false; }
    bool            isStandaloneCoordinate  (void) const final { return false; }
    bool            specifiesCoordinate     (void) const final;
    bool            specifiesAltitudeOnly   (void) const final { return false; }
    QString         commandDescription      (void) const final { return tr("Structure Scan"); }
    QString         commandName             (void) const final { return tr("Structure Scan"); }
    QString         abbreviation            (void) const final { return "S"; }
    QGeoCoordinate  coordinate              (void) const final { return _coordinate; }
    QGeoCoordinate  exitCoordinate          (void) const final { return _exitCoordinate; }
    int             sequenceNumber          (void) const final { return _sequenceNumber; }
    double          specifiedFlightSpeed    (void) final { return std::numeric_limits<double>::quiet_NaN(); }
    double          specifiedGimbalYaw      (void) final { return std::numeric_limits<double>::quiet_NaN(); }
    void            appendMissionItems      (QList<MissionItem*>& items, QObject* missionItemParent) final;
    void            setMissionFlightStatus  (MissionController::MissionFlightStatus_t& missionFlightStatus) final;
    void            applyNewAltitude        (double newAltitude) final;

    bool coordinateHasRelativeAltitude      (void) const final { return _altitudeRelativeFact.rawValue().toBool(); }
    bool exitCoordinateHasRelativeAltitude  (void) const final { return _altitudeRelativeFact.rawValue().toBool(); }
    bool exitCoordinateSameAsEntry          (void) const final { return false; }

    void setDirty           (bool dirty) final;
    void setCoordinate      (const QGeoCoordinate& coordinate) final;
    void setSequenceNumber  (int sequenceNumber) final;
    void save               (QJsonArray&  missionItems) final;

    static const char* jsonComplexItemTypeValue;

    static const char* settingsGroup;
    static const char* altitudeName;
    static const char* altitudeRelativeName;
    static const char* layersName;
    static const char* layerDistanceName;
    static const char* cameraTriggerDistanceName;

signals:
    void cameraShotsChanged             (int cameraShots);
    void timeBetweenShotsChanged        (void);
    void cameraMinTriggerIntervalChanged(double cameraMinTriggerInterval);

private slots:
    void _setDirty(void);
    void _polygonDirtyChanged(bool dirty);
    void _polygonCountChanged(int count);
    void _polygonPathChanged(void);
    void _clearInternal(void);

private:
    void _setExitCoordinate(const QGeoCoordinate& coordinate);
    void _updateCoordinateAltitude(void);
    void _setScanDistance(double scanDistance);
    void _setCameraShots(int cameraShots);
    double _triggerDistance(void) const;
    void _swapPoints(QList<QPointF>& points, int index1, int index2);
    qreal _ccw(QPointF pt1, QPointF pt2, QPointF pt3);
    qreal _dp(QPointF pt1, QPointF pt2);
    QList<QPointF> _convexPolygon(const QList<QPointF>& polygon);

    int             _sequenceNumber;
    bool            _dirty;
    QGCMapPolygon   _mapPolygon;
    QGeoCoordinate  _coordinate;
    QGeoCoordinate  _exitCoordinate;

    bool            _ignoreRecalc;
    double          _scanDistance;
    int             _cameraShots;
    double          _timeBetweenShots;
    double          _cameraMinTriggerInterval;
    double          _cruiseSpeed;

    QMap<QString, FactMetaData*> _metaDataMap;

    SettingsFact    _altitudeFact;
    SettingsFact    _altitudeRelativeFact;
    SettingsFact    _layersFact;
    SettingsFact    _layerDistanceFact;
    SettingsFact    _cameraTriggerDistanceFact;

    static const char* _jsonGridObjectKey;
    static const char* _jsonGridAltitudeKey;
    static const char* _jsonGridAltitudeRelativeKey;
    static const char* _jsonGridAngleKey;
    static const char* _jsonGridSpacingKey;
    static const char* _jsonGridEntryLocationKey;
    static const char* _jsonTurnaroundDistKey;
    static const char* _jsonCameraTriggerDistanceKey;
    static const char* _jsonCameraTriggerInTurnaroundKey;
    static const char* _jsonHoverAndCaptureKey;
    static const char* _jsonGroundResolutionKey;
    static const char* _jsonFrontalOverlapKey;
    static const char* _jsonSideOverlapKey;
    static const char* _jsonCameraSensorWidthKey;
    static const char* _jsonCameraSensorHeightKey;
    static const char* _jsonCameraResolutionWidthKey;
    static const char* _jsonCameraResolutionHeightKey;
    static const char* _jsonCameraFocalLengthKey;
    static const char* _jsonCameraMinTriggerIntervalKey;
    static const char* _jsonManualGridKey;
    static const char* _jsonCameraObjectKey;
    static const char* _jsonCameraNameKey;
    static const char* _jsonCameraOrientationLandscapeKey;
    static const char* _jsonFixedValueIsAltitudeKey;
    static const char* _jsonRefly90DegreesKey;

    static const int _hoverAndCaptureDelaySeconds = 1;
};

#endif
