BEGIN TRANSACTION ISOLATION LEVEL READ COMMITTED;
UPDATE Photos p
SET vehicleLicensePlate = c.vehicleLicensePlate,vehicleState = 'CA'
FROM PhotoChanges c
WHERE p.cameraID = c.cameraID AND p.photoTimestamp = c.photoTimestamp;


INSERT INTO Photos(cameraID,vehicleState,vehicleLicensePlate,photoTimestamp)
SELECT cameraID,NULL,vehicleLicensePlate,photoTimestamp
FROM PhotoChanges c
WHERE NOT EXISTS (SELECT cameraID, photoTimestamp 
              FROM Photos p
              WHERE c.cameraID = p.cameraID 
              AND c.photoTimestamp = p.photoTimestamp);
COMMIT;


