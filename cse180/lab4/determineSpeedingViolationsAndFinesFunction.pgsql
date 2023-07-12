CREATE OR REPLACE FUNCTION determineSpeedingViolationsAndFines(maxFineTotal INTEGER)
RETURNS INTEGER AS $$



    DECLARE 
        numViolationsLicense CHAR(8); -- what we will store our cursor in
        numViolationsState CHAR(2);
        numSpeedViolations INTEGER;
        maxFine INTEGER = 0; -- return value of the function
 --cursor to store which owner has comitted speed violations
    DECLARE ownerViolations CURSOR FOR
        SELECT o.ownerLicenseID, o.ownerState
        FROM Owners o, distancesAndIntervalsForPhotos d, Vehicles v, Highways h
        WHERE v.ownerLicenseID = o.ownerLicenseID
        AND v.ownerState = o.ownerState
        AND d.vehicleLicensePlate = v.vehicleLicensePlate
        AND d.highwayNum = h.highwayNum
        AND d.vehicleState = v.vehicleState
        AND (d.distBetweenCameraMileMarkers/d.photoIntervalInHours) > h.speedLimit;

 --cursor to reset the owner's fines
    -- DECLARE fineReset CURSOR FOR
    --     SELECT o.fine,o.speedingViolations
    --     FROM Owners o, distancesAndIntervalsForPhotos d, Vehicles v, Highways h
    --     WHERE v.ownerLicenseID = o.ownerLicenseID
    --     AND v.ownerState = o.ownerState
    --     AND d.vehicleLicensePlate = v.vehicleLicensePlate
    --     AND d.highwayNum = h.highwayNum
    --     AND d.vehicleState = v.vehicleState
    --     AND (d.distBetweenCameraMileMarkers/d.photoIntervalInHours) > h.speedLimit;


 --cursor to count how much to fine each owner (50,20,10)
    DECLARE ownerFines CURSOR FOR
        SELECT o.speedingViolations,o.ownerState,o.ownerLicenseID
        FROM Owners o
        GROUP BY o.ownerState,o.ownerLicenseID,o.fine
        HAVING COUNT(speedingViolations) > 0
        ORDER BY COUNT(*) DESC;

 --cursor to 
    BEGIN
    --input validation
        IF maxFineTotal <= 0 THEN
            return -1;
        END IF;
        OPEN ownerViolations;
        LOOP
            FETCH ownerViolations INTO numViolationsLicense,numViolationsState;
            EXIT WHEN NOT FOUND;
            UPDATE Owners 
            SET speedingViolations = speedingViolations + 1
            WHERE ownerLicenseID = numViolationsLicense
            AND ownerState = numVIolationsState;
        END LOOP;
        CLOSE ownerViolations;

        OPEN ownerFines;
        LOOP
            FETCH ownerFines INTO numSpeedViolations,numViolationsState,numViolationsLicense;
            EXIT WHEN NOT FOUND OR maxFineTotal <= maxFine;
            IF numSpeedViolations >= 3 THEN
                UPDATE Owners 
                SET fine = numSpeedViolations * 50
                WHERE ownerLicenseID = numViolationsLicense
                AND ownerState = numViolationsState;
                IF maxFine + (numSpeedViolations*50) <= maxFineTotal THEN 
                     maxFine = maxFine + (numSpeedViolations * 50);
                     END IF;
            END IF;
            IF numSpeedViolations = 2 THEN
                UPDATE Owners
                SET fine =  2*50
                WHERE ownerLicenseID = numViolationsLicense
                AND ownerState = numViolationsState;
                IF maxFine + (numSpeedViolations*20) <= maxFineTotal THEN 
                     maxFine = maxFine + (numSpeedViolations * 20);
                     END IF;
            END IF;
            IF numSpeedViolations = 1 THEN
                UPDATE Owners
                SET fine = 50
                WHERE ownerLicenseID = numViolationsLicense
                AND ownerState = numViolationsState;
                IF maxFine + 10 <= maxFineTotal THEN 
                     maxFine = maxFine + 10;
                     END IF;
            END IF;
        END LOOP;
        CLOSE ownerFines;
    -- Reset the tables
        OPEN ownerViolations;
        LOOP
        FETCH ownerViolations INTO numViolationsLicense,numViolationsState;
            EXIT WHEN NOT FOUND;
            UPDATE Owners 
            SET speedingViolations = 0, fine = 0
            WHERE ownerLicenseID = numViolationsLicense
            AND ownerState = numVIolationsState;
        END LOOP;
        CLOSE ownerViolations;
        
        RETURN maxFine;
    END
--end function
$$ LANGUAGE plpgsql;
--update owner's table with correct number of speeding violations
