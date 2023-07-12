SELECT v.vehicleLicensePlate as vehicleLicensePlate, o.ownerState as ownerState, year as year, expirationDate as expriationDate
FROM Vehicles v, Owners o
WHERE year < 2021
AND v.ownerState = o.ownerState
AND v.ownerLicenseID = o.ownerLicenseID
AND v.vehicleState = 'CA'
AND expirationDate IS NOT NULL
AND o.ownerState <> 'CA';