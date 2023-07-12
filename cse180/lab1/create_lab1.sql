DROP SCHEMA Lab1 CASCADE;
CREATE SCHEMA Lab1;

CREATE TABLE Highways(
    highwayNum integer,
    highwayLength numeric(4,1),
    speedLimit integer,
    PRIMARY KEY (highwayNum)
);

CREATE TABLE Exits(
    highwayNum integer, 
    exitNum integer, 
    exitDescription VARCHAR(60), 
    mileMarker numeric(4,1), 
    exitCity VARCHAR(20),
    exitState CHAR(2), 
    isExitOpen boolean,
    PRIMARY KEY (highwayNum,exitNum),
    FOREIGN KEY (highwayNum) REFERENCES Highways
);

CREATE TABLE Interchanges (
    highwayNum1 integer,
    exitNum1 integer, 
    highwayNum2 integer, 
    exitNum2 integer,
    PRIMARY KEY (highwayNum1,exitNum1,highwayNum2,exitNum2),
    FOREIGN KEY (highwayNum1,exitNum1) REFERENCES Exits,
    FOREIGN KEY (highwayNum2,exitNum2) REFERENCES Exits

);

CREATE TABLE Cameras (
    cameraID integer, 
    highwayNum integer, 
    mileMarker numeric(4,1), 
    isCameraWorking boolean,
    PRIMARY KEY (cameraID),
    FOREIGN KEY (highwayNum) REFERENCES Highways
);

CREATE TABLE Owners (
    ownerState CHAR(2), 
    ownerLicenseID CHAR(8), 
    ownerName VARCHAR(60), 
    ownerAddress VARCHAR(60), 
    startDate date, 
    expirationDate date,
    PRIMARY KEY (ownerState,ownerLicenseID)

);

CREATE TABLE Vehicles (
    vehicleState CHAR(2), 
    vehicleLicensePlate CHAR(7), 
    ownerState CHAR(2), 
    ownerLicenseID CHAR(8), 
    vehicleYear integer, 
    color CHAR(2),
    PRIMARY KEY (vehicleState,vehicleLicensePlate),
    FOREIGN KEY (ownerState,ownerLicenseID) REFERENCES Owners

);

CREATE TABLE Photos (
    cameraID integer, 
    vehicleState CHAR(2), 
    vehicleLicensePlate CHAR(7), 
    photoTimestamp timestamp,
    PRIMARY KEY (cameraID,photoTimestamp),
    FOREIGN KEY (vehicleState,vehicleLicensePlate) REFERENCES Vehicles
);