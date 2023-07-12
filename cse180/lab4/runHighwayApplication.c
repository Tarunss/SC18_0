/**
 * runHighwayApplication skeleton, to be modified by students
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "libpq-fe.h"

/* These constants would normally be in a header file */
/* Maximum length of string used to submit a connection */
#define MAXCONNECTIONSTRINGSIZE 501
/* Maximum length of string used to submit a SQL statement */
#define MAXSQLSTATEMENTSTRINGSIZE 2001
/* Maximum length of string version of integer; you don't have to use a value this big */
#define MAXNUMBERSTRINGSIZE 20

/* Exit with success after closing connection to the server
 *  and freeing memory that was used by the PGconn object.
 */
static void good_exit(PGconn *conn)
{
    PQfinish(conn);
    exit(EXIT_SUCCESS);
}

/* Exit with failure after closing connection to the server
 *  and freeing memory that was used by the PGconn object.
 */
static void bad_exit(PGconn *conn)
{
    PQfinish(conn);
    exit(EXIT_FAILURE);
}

/* The three C functions that for Lab4 should appear below.
 * Write those functions, as described in Lab4 Section 4 (and Section 5,
 * which describes the Stored Function used by the third C function).
 *
 * Write the tests of those function in main, as described in Section 6
 * of Lab4.
 *
 * You may use "helper" functions to avoid having to duplicate calls and
 * printing, if you'd like, but if Lab4 says do things in a function, do them
 * in that function, and if Lab4 says do things in main, do them in main,
 * possibly using a helper function, if you'd like.
 */

/* Function: printCameraPhotoCount:
 * -------------------------------------
 * Parameters:  connection, and theCameraID, which should be the ID of a camera.
 * Prints the cameraID, the highwayNum and mileMarker of that camera, and the
 * number of photos for that camera, if camera exists for thecameraID.
 * Return 0 if normal execution, -1 if no such camera.
 * bad_exit if SQL statement execution fails.
 */

int printCameraPhotoCount(PGconn *conn, int theCameraID)
{
    // begin transaction
    PGresult *res = PQexec(conn, "BEGIN TRANSACTION ISOLATION LEVEL SERIALIZABLE");
    // check to see if exec went through
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Bad command: BEGIN TRANSACTION\n");
        // clear result struct
        PQclear(res);
        // exit database
        bad_exit(conn);
        return (EXIT_FAILURE);
    }
    // clear res
    PQclear(res);

    // get camera ID into string format
    char strCameraID[MAXNUMBERSTRINGSIZE];
    sprintf(strCameraID, "%d", theCameraID);
    // now we have cameraID in string, we can query with it
    char sqlstatement1[MAXSQLSTATEMENTSTRINGSIZE] = "SELECT c.cameraID, c.highwayNum, c.mileMarker FROM Cameras c WHERE c.cameraID = ";
    // append our cameraID to sql statement
    strcat(sqlstatement1, strCameraID);
    PGresult *res_tuple = PQexec(conn, sqlstatement1);
    // check to see if worked
    if (PQresultStatus(res_tuple) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "Bad command: SELECT c.cameraID...\n");
        // clear result struct
        PQclear(res_tuple);
        // exit database
        bad_exit(conn);
        return (EXIT_FAILURE);
    }
    int cameraamount = PQntuples(res_tuple);
    if (cameraamount == 0)
    {
        PQclear(res_tuple);
        PGresult *res_rollback = PQexec(conn, "ROLLBACK TRANSACTION");
        if (PQresultStatus(res_rollback) != PGRES_COMMAND_OK)
        {
            fprintf(stderr, "ROLLBACK TRANSACTION failed: %s", PQerrorMessage(conn));
            PQclear(res_rollback);
            bad_exit(conn);
            return (-1);
        }
        PQclear(res_rollback);

        return -1;
    }
    // check to see if we even got tuples from query
    /* If number of tuples returned is 0, then there are no photos with that camera*/
    int numphotos;
    int tuples = PQntuples(res_tuple);
    if (tuples == 0)
    {
        numphotos = 0;
    }
    // get the strings of milemarker and highwaynum
    char *mileMarker;
    char *highwayNum;
    mileMarker = PQgetvalue(res_tuple, 0, 2);
    highwayNum = PQgetvalue(res_tuple, 0, 1);
    PQclear(res_tuple);

    // now that we have the tuples, we need to get the count of photos with same camera ID
    char sqlstatement2[MAXSQLSTATEMENTSTRINGSIZE] = "SELECT COUNT(*) FROM Photos p, Cameras c WHERE c.cameraID = p.cameraID AND p.cameraID = ";
    strcat(sqlstatement2, strCameraID);
    // need to add a group by
    // strcat(sqlstatement2, "GROUP BY p.cameraID");
    // execute SQL statement
    PGresult *res_count = PQexec(conn, sqlstatement2);
    // check result
    if (PQresultStatus(res_count) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "Bad command: SELECT COUNT(*)...\n");
        // clear result struct
        PQclear(res_count);
        // exit database
        bad_exit(conn);
        return (EXIT_FAILURE);
    }
    // check tuples again
    int numPhotos;
    int tuples2 = PQntuples(res_count);
    if (tuples == 0)
    {
        PQclear(res_count);
        PGresult *res_rollback = PQexec(conn, "ROLLBACK TRANSACTION");
        if (PQresultStatus(res_rollback) != PGRES_COMMAND_OK)
        {
            fprintf(stderr, "ROLLBACK TRANSACTION failed: %s", PQerrorMessage(conn));
            PQclear(res_rollback);
            bad_exit(conn);
            return (-1);
        }
        PQclear(res_rollback);

        return -1;
    }
    numPhotos = atoi(PQgetvalue(res_count, 0, 0));
    PQclear(res_count);
    // cast and print result
    printf("CameraID: %d, highwayNum: %s, mileMarker: %s, numPhotos: %d\n", theCameraID, highwayNum, mileMarker, numPhotos);

    // end transaction
    PGresult *res_commit = PQexec(conn, "COMMIT TRANSACTION");
    if (PQresultStatus(res_commit) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "COMMIT TRANSACTION failed: %s", PQerrorMessage(conn));
        PQclear(res_commit);
        bad_exit(conn);
        return (-1);
    }
    PQclear(res_commit);
    return (0);
}

/* Function: openAllExits:
 * ----------------------------
 * Parameters:  connection, and theHighwayNum, the number of a highway.

 * Opens all the exit on that highway that weren't already open.
 * Returns the number of exits on the highway that weren't open,
 * or -1 if there is no highway whose highwayNum is theHighwayNum.
 */

int openAllExits(PGconn *conn, int theHighwayNum)
{
    // commit transaciton
    PGresult *res = PQexec(conn, "BEGIN TRANSACTION ISOLATION LEVEL SERIALIZABLE");
    // check to see if exec went through
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Bad command: BEGIN TRANSACTION\n");
        // clear result struct
        PQclear(res);
        // exit database
        bad_exit(conn);
        return (-1);
    }
    PQclear(res);

    // convert into string to append
    char highwayNum[MAXNUMBERSTRINGSIZE];
    sprintf(highwayNum, "%d", theHighwayNum);

    // get sql string
    char sqlcheckHighways[MAXSQLSTATEMENTSTRINGSIZE] = "SELECT h.highwayNum FROM highways h WHERE h.highwayNum = ";
    strcat(sqlcheckHighways, highwayNum);
    // printf("WHERE IS SEG FAULT\n");

    // check to see if there is a highway with that number
    PGresult *res_check = PQexec(conn, sqlcheckHighways);
    if (PQresultStatus(res_check) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "Bad command: SELECT h.highwayNum...\n");
        // clear result struct
        PQclear(res_check);
        // exit database
        bad_exit(conn);
        return (-1);
    }
    int count = PQntuples(res_check);
    // check to see if any highways with that number
    if (count == 0)
    {
        // there aren't any highways with that number
        // fprintf(stderr, "Error: no highways with highway number %d\n", theHighwayNum);
        PQclear(res_check);
        PGresult *res_commit = PQexec(conn, "COMMIT TRANSACTION");
        if (PQresultStatus(res_commit) != PGRES_COMMAND_OK)
        {
            fprintf(stderr, "COMMIT TRANSACTION failed: %s", PQerrorMessage(conn));
            PQclear(res_commit);
            bad_exit(conn);
            return (-1);
        }
        PQclear(res_commit);
        return (-1);
    }
    // printf("WHERE IS SEG FAULT 2\n");
    //  update that table with correct statement
    char sqlupdateexits[MAXSQLSTATEMENTSTRINGSIZE] = "UPDATE Exits SET isExitOpen = TRUE WHERE isExitOpen = FALSE AND Exits.highwayNum = ";
    strcat(sqlupdateexits, highwayNum);
    PGresult *res_update = PQexec(conn, sqlupdateexits);
    if (PQresultStatus(res_update) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "UPDATE failed: %s, %s", sqlupdateexits, PQerrorMessage(conn));
        PQclear(res_update);
        bad_exit(conn);
        return (-1);
    }
    // obtain amount of exits that have been updated
    int update_count = atoi(PQcmdTuples(res_update));
    // printf("WHERE IS SEG FAULT 3\n");
    //  print out number of columnds updated
    PQclear(res_update);
    // int returnval = 0;
    // memcpy(&returnval, &update_count, sizeof(int));
    // end transaction
    PGresult *res_commit = PQexec(conn, "COMMIT TRANSACTION");
    if (PQresultStatus(res_commit) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "COMMIT TRANSACTION failed: %s", PQerrorMessage(conn));
        PQclear(res_commit);
        bad_exit(conn);
        return (-1);
    }
    PQclear(res_commit);
    // printf("%d\n", update_count);
    return (update_count);
}

/* Function: determineSpeedingViolationsAndFines:
 * -------------------------------
 * Parameters:  connection, and an integer maxFineTotal, the maximum total
 * of the fines that should be assessed to owners whose vehicles were speeding.
 *
 * It should count the number of speeding violations by vehicles that each owner
 * owns, and set the speedingViolations field of Owners accordingly.
 *
 * It should also assess fines to some owners based on the number of speeding
 * violations they have.
 *
 * Executes by invoking a Stored Function,
 * determineSpeedingViolationsAndFinesFunction, which does all of the work,
 * as described in Section 5 of Lab4.
 *
 * Returns a negative value if there is an error, and otherwise returns the total
 * fines that were assessed by the Stored Function.
 */

int determineSpeedingViolationsAndFines(PGconn *conn, int maxFineTotal)
{
    // commit transaciton
    PGresult *res = PQexec(conn, "BEGIN TRANSACTION ISOLATION LEVEL SERIALIZABLE");
    // check to see if exec went through
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Bad command: BEGIN TRANSACTION\n");
        // clear result struct
        PQclear(res);
        // exit database
        bad_exit(conn);
        return (-1);
    }

    // call stored function
    char num[MAXNUMBERSTRINGSIZE];
    char func[MAXSQLSTATEMENTSTRINGSIZE] = "SELECT determineSpeedingViolationsAndFines(";
    sprintf(num, "%d", maxFineTotal);
    strcat(func, num);
    strcat(func, ")");
    PGresult *res_function = PQexec(conn, func);
    if (PQresultStatus(res_function) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "FUNCTION determineSpeedingViolationsAndFines failed on %d: %s/n", maxFineTotal, PQerrorMessage(conn));
        PQclear(res_function);
        bad_exit(conn);
    }

    int k = atoi(PQgetvalue(res_function, 0, 0));

    PQclear(res);

    PGresult *res_commit = PQexec(conn, "COMMIT TRANSACTION");
    if (PQresultStatus(res_commit) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "COMMIT TRANSACTION failed: %s", PQerrorMessage(conn));
        PQclear(res_commit);
        bad_exit(conn);
        return (-1);
    }
    PQclear(res_commit);
    return (k);
}

int main(int argc, char **argv)
{
    PGconn *conn;
    int theResult;

    if (argc != 3)
    {
        fprintf(stderr, "Usage: ./runHighwayApplication <username> <password>\n");
        exit(EXIT_FAILURE);
    }

    char *userID = argv[1];
    char *pwd = argv[2];

    char conninfo[MAXCONNECTIONSTRINGSIZE] = "host=cse180-db.lt.ucsc.edu user=";
    strcat(conninfo, userID);
    strcat(conninfo, " password=");
    strcat(conninfo, pwd);

    /* Make a connection to the database */
    conn = PQconnectdb(conninfo);

    /* Check to see if the database connection was successfully made. */
    if (PQstatus(conn) != CONNECTION_OK)
    {
        fprintf(stderr, "Connection to database failed: %s\n",
                PQerrorMessage(conn));
        bad_exit(conn);
    }

    int result;

    /* Perform the calls to printCameraPhotoCount listed in Section 6 of Lab4,
     * printing error message if there's an error.
     */
    int cameraPhotoCount = printCameraPhotoCount(conn, 951);
    if (cameraPhotoCount == -1)
    {
        printf("No camera exists whose ID is %d\n", 951);
    }
    cameraPhotoCount = printCameraPhotoCount(conn, 960);
    if (cameraPhotoCount == -1)
    {
        printf("No camera exists whose ID is %d\n", 960);
    }
    cameraPhotoCount = printCameraPhotoCount(conn, 856);
    if (cameraPhotoCount == -1)
    {
        printf("No camera exists whose ID is %d\n", 856);
    }
    cameraPhotoCount = printCameraPhotoCount(conn, 904);
    if (cameraPhotoCount == -1)
    {
        printf("No camera exists whose ID is %d\n", 904);
    }
    /* Extra newline for readability */
    printf("\n");

    /* Perform the calls to openAllExits listed in Section 6 of Lab4,
     * and print messages as described.
     */
    int exitsOpened;
    exitsOpened = openAllExits(conn, 101);
    if (exitsOpened >= 0)
    {
        printf("%d exits opened by openAllExits\n", exitsOpened);
    }
    else
    {
        printf("There is no highway whose number is %d\n", 101);
    }
    exitsOpened = openAllExits(conn, 13);
    if (exitsOpened >= 0)
    {
        printf("%d exits opened by openAllExits\n", exitsOpened);
    }
    else
    {
        printf("There is no highway whose number is %d\n", 13);
    }
    exitsOpened = openAllExits(conn, 280);
    if (exitsOpened >= 0)
    {
        printf("%d exits opened by openAllExits\n", exitsOpened);
    }
    else
    {
        printf("There is no highway whose number is %d\n", 280);
    }
    exitsOpened = openAllExits(conn, 17);
    if (exitsOpened >= 0)
    {
        printf("%d exits opened by openAllExits\n", exitsOpened);
    }
    else
    {
        printf("There is no highway whose number is %d\n", 17);
    }
    /* Extra newline for readability */
    printf("\n");

    /* Perform the calls to determineSpeedingViolationsAndFines listed in Section
     * 6 of Lab4, and print messages as described.
     * You may use helper functions to do this, if you want.
     */
    printf("Total fines for maxFineTotal %d is %d\n", 300, determineSpeedingViolationsAndFines(conn, 300));
    printf("Total fines for maxFineTotal %d is %d\n", 240, determineSpeedingViolationsAndFines(conn, 240));
    printf("Total fines for maxFineTotal %d is %d\n", 210, determineSpeedingViolationsAndFines(conn, 210));
    printf("Total fines for maxFineTotal %d is %d\n", 165, determineSpeedingViolationsAndFines(conn, 165));
    good_exit(conn);
    return 0;
}
