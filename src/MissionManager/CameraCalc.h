/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#pragma once

#include "CameraSpec.h"

class CameraCalc : public CameraSpec
{
    Q_OBJECT

public:
    CameraCalc(QObject* parent = NULL);

    Q_ENUMS(CameraSpecType)

    Q_PROPERTY(CameraSpecType   cameraSpecType      MEMBER  _cameraSpecType     NOTIFY cameraSpecTypeChanged)
    Q_PROPERTY(QString          knownCameraName     MEMBER  _knownCameraName    NOTIFY knownCameraNameChanged)
    Q_PROPERTY(Fact*            valueSetIsDistance  READ    valueSetIsDistance  CONSTANT)                       ///< true: distance specified, resolution calculated
    Q_PROPERTY(Fact*            distanceToSurface   READ    distanceToSurface   CONSTANT)                       ///< Distance to surface for image foot print calculation
    Q_PROPERTY(Fact*            imageDensity        READ    imageDensity        CONSTANT)                       ///< Image density on surface (cm/px)

    // The following values are calculated from the camera properties
    Q_PROPERTY(double surfaceImageSizeSide      READ surfaceImageSizeSide       NOTIFY surfaceImageSizeSideChanged)
    Q_PROPERTY(double surfaceImageSizeFrontal   READ surfaceImageSizeFrontal    NOTIFY surfaceImageSizeFrontalChanged)

    enum CameraSpecType {
        cameraSpecNone,
        cameraSpecCustom,
        cameraSpecKnown
    };

    Fact* valueSetIsDistance(void) { return &_valueSetIsDistanceFact; }
    Fact* distanceToSurface (void) { return &_distanceToSurfaceFact; }
    Fact* imageDensity      (void) { return &_imageDensityFact; }

    double surfaceImageSizeSide     (void) const { return _surfaceImageSizeSide; }
    double surfaceImageSizeFrontal  (void) const { return _surfaceImageSizeFrontal; }

    bool dirty      (void) const { return _dirty; }
    void setDirty   (bool dirty);

    void save(QJsonObject& json) const;
    bool load(const QJsonObject& json, QString& errorString);

signals:
    void dirtyChanged                   (bool dirty);
    void surfaceImageSizeSideChanged    (double surfaceImageSizeSide);
    void surfaceImageSizeFrontalChanged (double surfaceImageSizeFrontal);

private:
    bool _dirty;

    QMap<QString, FactMetaData*> _metaDataMap;

    Fact _valueSetIsDistanceFact;
    Fact _distanceToSurfaceFact;
    Fact _imageDensityFact;

    double _surfaceImageSizeSide;
    double _surfaceImageSizeFrontal;

    static const char* _valueSetIsDistanceName;
    static const char* _distanceToSurfaceName;
    static const char* _imageDensityName;
};
