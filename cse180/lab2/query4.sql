SELECT c.cameraID as theCameraId, photoTimestamp as theTimestamp, name as theOwnerName,address as theOwnerAddress
FROM Photos p, Cameras c, Vehicles v, Owners o, Highways h
WHERE c.cameraID = p.cameraID
AND v.ownerLicenseID = o.ownerLicenseID
AND h.highwayNum = c.highwayNum
AND p.vehicleLicensePlate = v.vehicleLicensePlate
AND isCameraWorking IS TRUE
AND DATE(photoTimestamp) = '2022-12-01'
AND (color = 'RE' OR color = 'GR')
AND name LIKE '_o%'
AND speedLimit <= 65;
