INSERT INTO Vehicles(vehicleState,vehicleLicensePlate,ownerState,ownerLicenseID)
VALUES ('NJ','3XYZ124','NJ','P423553');

INSERT INTO Photos(cameraID,photoTimestamp,vehicleState,vehicleLicensePlate)
VALUES (973,'2022-12-01 16:20:01','CA','3XYZ121');

INSERT INTO Photos(cameraID,photoTimestamp,vehicleState,vehicleLicensePlate)
VALUES (309,'2022-12-01 16:20:01','CA','3XYZ123');

UPDATE Exits
SET mileMarker = 1
WHERE highwayNum = 17;

UPDATE Exits
SET mileMarker = -1
WHERE highwayNum = 17;
 
UPDATE Interchanges
SET highwayNum1 = 1, highwayNum2 = 17
WHERE exitNum1 = 1 AND exitNum2 = 1;

UPDATE Interchanges
SET highwayNum1 = 17, highwayNum2 = 1
WHERE exitNum1 = 1 AND exitNum2 = 1;

UPDATE Owners
SET startDate = '2023-06-02'
WHERE expirationDate IS NULL;

UPDATE Owners
SET startDate = NULL
WHERE expirationDate = '2023-06-02'