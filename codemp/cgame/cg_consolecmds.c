// Copyright (C) 1999-2000 Id Software, Inc.
//
// cg_consolecmds.c -- text commands typed in at the local console, or
// executed by a key binding

#ifdef WIN32

#if (defined(_M_IX86) || defined(_M_IA64) || defined(_M_AMD64)) && !defined(MIDL_PASS)
#define DECLSPEC_IMPORT __declspec(dllimport)
#else
#define DECLSPEC_IMPORT
#endif

#if !defined(_GDI32_)
#define WINGDIAPI DECLSPEC_IMPORT
#else
#define WINGDIAPI
#endif
#define WINAPI      __stdcall
#define APIENTRY WINAPI

#endif

#include <gl/gl.h>

#include "cg_local.h"
#include "../ui/ui_shared.h"
#include "bg_saga.h"
extern menuDef_t *menuScoreboard;



void CG_TargetCommand_f( void ) {
	int		targetNum;
	char	test[4];

	targetNum = CG_CrosshairPlayer();
	if (!targetNum ) {
		return;
	}

	trap_Argv( 1, test, 4 );
	trap_SendConsoleCommand( va( "gc %i %i", targetNum, atoi( test ) ) );
}



/*
=================
CG_SizeUp_f

Keybinding command
=================
*/
static void CG_SizeUp_f (void) {
	trap_Cvar_Set("cg_viewsize", va("%i",(int)(cg_viewsize.integer+10)));
}


/*
=================
CG_SizeDown_f

Keybinding command
=================
*/
static void CG_SizeDown_f (void) {
	trap_Cvar_Set("cg_viewsize", va("%i",(int)(cg_viewsize.integer-10)));
}


/*
=============
CG_Viewpos_f

Debugging command to print the current position
=============
*/
static void CG_Viewpos_f (void) {
	CG_Printf ("%s origin (%f %f %f) : angles (%f %f %f)\n", cgs.mapname, cg.refdef.vieworg[0],
		cg.refdef.vieworg[1], cg.refdef.vieworg[2], 
		cg.refdef.viewangles[0], cg.refdef.viewangles[1], cg.refdef.viewangles[2]);
}


static void CG_ScoresDown_f( void ) {

	CG_BuildSpectatorString();
	if ( cg.scoresRequestTime + 2000 < cg.time ) {
		// the scores are more than two seconds out of data,
		// so request new ones
		cg.scoresRequestTime = cg.time;
		trap_SendClientCommand( "score" );

		// leave the current scores up if they were already
		// displayed, but if this is the first hit, clear them out
		if ( !cg.showScores ) {
			cg.showScores = qtrue;
			cg.numScores = 0;
		}
	} else {
		// show the cached contents even if they just pressed if it
		// is within two seconds
		cg.showScores = qtrue;
	}
}

static void CG_ScoresUp_f( void ) {
	if ( cg.showScores ) {
		cg.showScores = qfalse;
		cg.scoreFadeTime = cg.time;
	}
}

extern menuDef_t *menuScoreboard;
void Menu_Reset();			// FIXME: add to right include file

static void CG_scrollScoresDown_f( void) {
	if (menuScoreboard && cg.scoreBoardShowing) {
		Menu_ScrollFeeder(menuScoreboard, FEEDER_SCOREBOARD, qtrue);
		Menu_ScrollFeeder(menuScoreboard, FEEDER_REDTEAM_LIST, qtrue);
		Menu_ScrollFeeder(menuScoreboard, FEEDER_BLUETEAM_LIST, qtrue);
	}
}


static void CG_scrollScoresUp_f( void) {
	if (menuScoreboard && cg.scoreBoardShowing) {
		Menu_ScrollFeeder(menuScoreboard, FEEDER_SCOREBOARD, qfalse);
		Menu_ScrollFeeder(menuScoreboard, FEEDER_REDTEAM_LIST, qfalse);
		Menu_ScrollFeeder(menuScoreboard, FEEDER_BLUETEAM_LIST, qfalse);
	}
}


static void CG_spWin_f( void) {
	trap_Cvar_Set("cg_cameraOrbit", "2");
	trap_Cvar_Set("cg_cameraOrbitDelay", "35");
	trap_Cvar_Set("cg_thirdPerson", "1");
	trap_Cvar_Set("cg_thirdPersonAngle", "0");
	trap_Cvar_Set("cg_thirdPersonRange", "100");
	CG_AddBufferedSound(cgs.media.winnerSound);
	//trap_S_StartLocalSound(cgs.media.winnerSound, CHAN_ANNOUNCER);
	CG_CenterPrint(CG_GetStringEdString("MP_INGAME", "YOU_WIN"), SCREEN_HEIGHT * .30, 0);
}

static void CG_spLose_f( void) {
	trap_Cvar_Set("cg_cameraOrbit", "2");
	trap_Cvar_Set("cg_cameraOrbitDelay", "35");
	trap_Cvar_Set("cg_thirdPerson", "1");
	trap_Cvar_Set("cg_thirdPersonAngle", "0");
	trap_Cvar_Set("cg_thirdPersonRange", "100");
	CG_AddBufferedSound(cgs.media.loserSound);
	//trap_S_StartLocalSound(cgs.media.loserSound, CHAN_ANNOUNCER);
	CG_CenterPrint(CG_GetStringEdString("MP_INGAME", "YOU_LOSE"), SCREEN_HEIGHT * .30, 0);
}


static void CG_TellTarget_f( void ) {
	int		clientNum;
	char	command[128];
	char	message[128];

	clientNum = CG_CrosshairPlayer();
	if ( clientNum == -1 ) {
		return;
	}

	trap_Args( message, 128 );
	Com_sprintf( command, 128, "tell %i %s", clientNum, message );
	trap_SendClientCommand( command );
}

static void CG_TellAttacker_f( void ) {
	int		clientNum;
	char	command[128];
	char	message[128];

	clientNum = CG_LastAttacker();
	if ( clientNum == -1 ) {
		return;
	}

	trap_Args( message, 128 );
	Com_sprintf( command, 128, "tell %i %s", clientNum, message );
	trap_SendClientCommand( command );
}


/*
==================
CG_StartOrbit_f
==================
*/

static void CG_StartOrbit_f( void ) {
	char var[MAX_TOKEN_CHARS];

	trap_Cvar_VariableStringBuffer( "developer", var, sizeof( var ) );
	if ( !atoi(var) ) {
		return;
	}
	if (cg_cameraOrbit.value != 0) {
		trap_Cvar_Set ("cg_cameraOrbit", "0");
		trap_Cvar_Set("cg_thirdPerson", "0");
	} else {
		trap_Cvar_Set("cg_cameraOrbit", "5");
		trap_Cvar_Set("cg_thirdPerson", "1");
		trap_Cvar_Set("cg_thirdPersonAngle", "0");
		trap_Cvar_Set("cg_thirdPersonRange", "100");
	}
}

void CG_SiegeBriefingDisplay(int team, int dontshow);
static void CG_SiegeBriefing_f(void)
{
	int team;

	if (cgs.gametype != GT_SIEGE)
	{ //Cannot be displayed unless in this gametype
		return;
	}

	team = cg.predictedPlayerState.persistant[PERS_TEAM];

	if (team != SIEGETEAM_TEAM1 &&
		team != SIEGETEAM_TEAM2)
	{ //cannot be displayed if not on a valid team
		return;
	}

	CG_SiegeBriefingDisplay(team, 0);
}

static void CG_SiegeCvarUpdate_f(void)
{
	int team;

	if (cgs.gametype != GT_SIEGE)
	{ //Cannot be displayed unless in this gametype
		return;
	}

	team = cg.predictedPlayerState.persistant[PERS_TEAM];

	if (team != SIEGETEAM_TEAM1 &&
		team != SIEGETEAM_TEAM2)
	{ //cannot be displayed if not on a valid team
		return;
	}

	CG_SiegeBriefingDisplay(team, 1);
}
static void CG_SiegeCompleteCvarUpdate_f(void)
{

	if (cgs.gametype != GT_SIEGE)
	{ //Cannot be displayed unless in this gametype
		return;
	}

	// Set up cvars for both teams
	CG_SiegeBriefingDisplay(SIEGETEAM_TEAM1, 1);
	CG_SiegeBriefingDisplay(SIEGETEAM_TEAM2, 1);
}
/*
static void CG_Camera_f( void ) {
	char name[1024];
	trap_Argv( 1, name, sizeof(name));
	if (trap_loadCamera(name)) {
		cg.cameraMode = qtrue;
		trap_startCamera(cg.time);
	} else {
		CG_Printf ("Unable to load camera %s\n",name);
	}
}
*/
static void CG_clientserverstatus_f(void)
{
	int clientNum;
	const char *configstring;
	for(clientNum=0;clientNum<cgs.maxclients;clientNum++)
	{
    	configstring = CG_ConfigString( clientNum + CS_PLAYERS );
	    CG_Printf("%d: %s\n",clientNum,Info_ValueForKey(configstring, "n"));
    }
}

int gotime=0;

static void CG_demopos_f(void)
{
	char min[20],*sec;
	int i;
	if(!cg.demoPlayback)return;
	if( trap_Argc() != 2 )
	{
		CG_Printf( "Syntax: demopos mm:ss.ms (seconds and ms are omittable)\n" );
		return;
	}
	trap_Argv( 1, min, 20 );
	for( i=0; min[i]!=':'&& min[i]!=0; i++ );
	if(min[i]==0)
		sec = 0;
	else
	{
		min[i] = 0;
		sec = min+i+1;
	}
	gotime = ( atoi( min ) * 60000 + ( sec ? atof( sec ) : 0 ) * 1000 );
	trap_Cvar_Set("timescale",va("%d",gotime - (cg.time - cgs.levelStartTime)));
}

typedef struct {
	double		coefficient[3];
	vec4_t		csi_coeff;   //cubic spline interpolation coefficients for time
} newCam_t;

newCam_t newcam[50];

typedef struct {
	vec3_t		origin;
	vec3_t		angle;
	int			time;
	qboolean	thirdPerson;
	vec4_t		csi_coeff[6];   //cubic spline interpolation coefficients
} demoCam_t;

demoCam_t cam[50];
int curCam = 0;

// start cubic spline interpolation code (parts are converted from javascript at http://people.hofstra.edu/faculty/Stefan_Waner/realworld/tutorialsf1/scriptpivot2.html)

double theMatrix[60*4][60*4+1];
int numRows = 0, numCols = 0;

static void pivot( int rows, int cols, int theRow, int theCol ) {
	double thePivot = theMatrix[theRow][theCol];
	int i, j;
	for( i=0; i<cols; i++ )
		theMatrix[theRow][i] = theMatrix[theRow][i]/thePivot;
	for( i=0; i<rows; i++ )
		if( ( i!=theRow ) && ( theMatrix[i][theCol]!=0 ) )
		{
			double factr = theMatrix[i][theCol];
			for( j=0; j<cols; j++ )
				theMatrix[i][j] = theMatrix[i][j] - factr*theMatrix[theRow][j];	
		}
}

static void swapRows( int p, int q ) {
	double rowHold = 0;
	int j;
	for( j=0; j<numCols; j++ )
	{
		rowHold = theMatrix[p][j];
		theMatrix[p][j] = theMatrix[q][j];
		theMatrix[q][j] = rowHold;
	}
}

static void rowReduce() {
	int i, j, theCol;
	double theSmallestNumber = 0.00000000001;
	int theRow = 0;
	for(  i=0; i<numRows; i++ )
	{ 
		theCol = -1;
		for( j=0; j<numCols; j++ )
		{
			double theNum = theMatrix[i][j];
			if( theNum * ( theNum>0?1:-1 ) <= theSmallestNumber )theMatrix[i][j] = 0;
			else { theCol = j; break; }
		}
		if( theCol!=-1 )pivot(numRows,numCols,i,theCol);
	}
	for( j=0; j<numCols; j++ )
		for( i=theRow; i<numRows; i++ )
			if( theMatrix[i][j] != 0 )
			{
				if( i==theRow ) { theRow++; break; }
				else { swapRows( theRow, i ); theRow++; break; }
			}
}

// this could be sped up with memoization
static double factorial(int n) {
	double product = 1;
	int i;
	for (i = 2; i <= n; i++) {
		product *= i;
	}
	return product;
}

static double choose(int numerator, int denominator) {
	return factorial(numerator) / factorial(denominator) / factorial(numerator - denominator);
}

#define TIMEDIV 1000
// recalculate cubic spline interpolation coefficients for cam paths
/*static */void reCalc() {
	int i, j, k, curRow;
	for( k=0; k<3; k++ )
	{
		double deltaT;

		{
			CG_Printf("curCam %d\n", curCam);
			for (i = 0; i < curCam; i++) {
				newcam[i].coefficient[k] = cam[i].origin[k] * choose(curCam - 1, i);
				CG_Printf("cam %d axis %d coefficient %lf\n", i, k, newcam[i].coefficient[k]);
			}

			// ******************************** FIX ME PLEASE ***************************************

			curRow = 0;
			memset( theMatrix[0], 0, sizeof( theMatrix ) );
			for( i=0; i<curCam-1; i++ )
			{
				double deltaT = (double)(cam[i+1].time - cam[i].time) / (double) (cam[curCam - 1].time - cam[0].time);
				double deltaD = (double)(1 / (double) (curCam - 1));
				j=0;
				//runs through first point
				//guess what? ive decided it's (0,0) kthx.
				theMatrix[curRow][i*4+j++] = 0;//(((double)cam[i].time))*(((double)cam[i].time))*(((double)cam[i].time));
				theMatrix[curRow][i*4+j++] = 0;//(((double)cam[i].time))*(((double)cam[i].time));
				theMatrix[curRow][i*4+j++] = 0;//(((double)cam[i].time));
				theMatrix[curRow][i*4+j++] = 1;
				theMatrix[curRow++][(curCam-1)*4+4] = 0;//cam[i].origin[k];
			
				j=0;
				//runs through second point
				//doodz we're makin dese thangs unit doodz so it b leeter :>
				theMatrix[curRow][i*4+j++] = (((double)deltaT))*(((double)deltaT))*(((double)deltaT));
				theMatrix[curRow][i*4+j++] = (((double)deltaT))*(((double)deltaT));
				theMatrix[curRow][i*4+j++] = (((double)deltaT));
				theMatrix[curRow][i*4+j++] = 1;
				theMatrix[curRow++][(curCam-1)*4+4] = deltaD;
			
				j=0;
				//first derivitives are equal
				theMatrix[curRow][i*4+j++] = (((double)deltaT))*(((double)deltaT))*3;
				theMatrix[curRow][i*4+j++] = (((double)deltaT))*2;
				theMatrix[curRow][i*4+j++] = 1;
				theMatrix[curRow][i*4+j++] = 0;
				theMatrix[curRow][i*4+j++] = 0;//-(((double)deltaT))*(((double)deltaT))*3;
				theMatrix[curRow][i*4+j++] = 0;//-(((double)deltaT))*2;
				theMatrix[curRow][i*4+j++] = -1;
				theMatrix[curRow][i*4+j++] = 0;
				theMatrix[curRow++][(curCam-1)*4+4] = 0;
			
				j=0;
				//second derivitives are equal
				theMatrix[curRow][i*4+j++] = (((double)deltaT))*6;
				theMatrix[curRow][i*4+j++] = 2;
				theMatrix[curRow][i*4+j++] = 0;
				theMatrix[curRow][i*4+j++] = 0;
				theMatrix[curRow][i*4+j++] = 0;//-(((double)deltaT))*6;
				theMatrix[curRow][i*4+j++] = -2;
				theMatrix[curRow][i*4+j++] = 0;
				theMatrix[curRow][i*4+j++] = 0;
				theMatrix[curRow++][(curCam-1)*4+4] = 0;
			}
			//4 more equations are needed to have a solvable matrix (there should be n-1 splines)
		
			j=0;
			//second derivitive at the end is zero
			{
				double deltaT = (double)(cam[curCam - 1].time - cam[curCam - 2].time) / (double) (cam[curCam - 1].time - cam[0].time);
				double deltaD = (double)(1 / (double) curCam);
				theMatrix[curRow][(curCam-1)*4+j++] = (((double)deltaT))*6;
				theMatrix[curRow][(curCam-1)*4+j++] = 2;
				theMatrix[curRow][(curCam-1)*4+j++] = 0;
				theMatrix[curRow][(curCam-1)*4+j++] = 0;
				theMatrix[curRow++][(curCam-1)*4+4] = 0;
			}
		
			j=0;
			//second derivitive at the beginning is zero
			theMatrix[curRow][j++] = 0;//(((double)cam[0].time))*6;
			theMatrix[curRow][j++] = 2;
			theMatrix[curRow][j++] = 0;
			theMatrix[curRow][j++] = 0;
			theMatrix[curRow++][(curCam-1)*4+4] = 0;
		
			j=0;
			//runs through first point
			theMatrix[curRow][(curCam-1)*4+j++] = 0;//(((double)cam[curCam-1].time)/TIMEDIV)*(((double)cam[curCam-1].time)/TIMEDIV)*(((double)cam[curCam-1].time)/TIMEDIV);
			theMatrix[curRow][(curCam-1)*4+j++] = 0;//(((double)cam[curCam-1].time)/TIMEDIV)*(((double)cam[curCam-1].time)/TIMEDIV);
			theMatrix[curRow][(curCam-1)*4+j++] = 0;//(((double)cam[curCam-1].time)/TIMEDIV);
			theMatrix[curRow][(curCam-1)*4+j++] = 1;
			theMatrix[curRow++][(curCam-1)*4+4] = 0;//cam[(curCam-1)].origin[k];
		
			// deltaT = (double)(cam[curCam].time - cam[curCam-1].time);
			j=0;
			//runs through second point
			//pretty sure that's supposed to say Last point... =/
			{
				double deltaT = (double)(cam[curCam - 1].time - cam[curCam - 2].time) / (double) (cam[curCam - 1].time - cam[0].time);
				double deltaD = (double)(1 / (double) (curCam - 1));
				theMatrix[curRow][(curCam-1)*4+j++] = (((double)deltaT))*(((double)deltaT))*(((double)deltaT));
				theMatrix[curRow][(curCam-1)*4+j++] = (((double)deltaT))*(((double)deltaT));
				theMatrix[curRow][(curCam-1)*4+j++] = (((double)deltaT));
				theMatrix[curRow][(curCam-1)*4+j++] = 1;
				theMatrix[curRow++][(curCam-1)*4+4] = deltaD;
			}
		
			numRows = curRow;
			numCols = curCam*4+1;
			rowReduce();
		
			for( i=0; i<curCam; i++ )
			{
				newcam[i].csi_coeff[0] = theMatrix[i*4][curCam*4];
				newcam[i].csi_coeff[1] = theMatrix[i*4+1][curCam*4];
				newcam[i].csi_coeff[2] = theMatrix[i*4+2][curCam*4];
				newcam[i].csi_coeff[3] = theMatrix[i*4+3][curCam*4];
			}

			// ************************************* FIX THE ABOVE PLEASE *****************************************
		}

		curRow = 0;
		memset( theMatrix[0], 0, sizeof( theMatrix ) );
		for( i=0; i<curCam-1; i++ )
		{
			double deltaT = (double)(cam[i+1].time - cam[i].time);
			double deltaD = (double)(cam[i+1].origin[k] - cam[i].origin[k]);
			j=0;
			//runs through first point
			//guess what? ive decided it's (0,0) kthx.
			theMatrix[curRow][i*4+j++] = 0;//(((double)cam[i].time)/TIMEDIV)*(((double)cam[i].time)/TIMEDIV)*(((double)cam[i].time)/TIMEDIV);
			theMatrix[curRow][i*4+j++] = 0;//(((double)cam[i].time)/TIMEDIV)*(((double)cam[i].time)/TIMEDIV);
			theMatrix[curRow][i*4+j++] = 0;//(((double)cam[i].time)/TIMEDIV);
			theMatrix[curRow][i*4+j++] = 1;
			theMatrix[curRow++][(curCam-1)*4+4] = 0;//cam[i].origin[k];
			
			j=0;
			//runs through second point
			//doodz we're makin dese thangs unit doodz so it b leeter :>
			theMatrix[curRow][i*4+j++] = (((double)deltaT)/TIMEDIV)*(((double)deltaT)/TIMEDIV)*(((double)deltaT)/TIMEDIV);
			theMatrix[curRow][i*4+j++] = (((double)deltaT)/TIMEDIV)*(((double)deltaT)/TIMEDIV);
			theMatrix[curRow][i*4+j++] = (((double)deltaT)/TIMEDIV);
			theMatrix[curRow][i*4+j++] = 1;
			theMatrix[curRow++][(curCam-1)*4+4] = deltaD;
			
			j=0;
			//first derivitives are equal
			theMatrix[curRow][i*4+j++] = (((double)deltaT)/TIMEDIV)*(((double)deltaT)/TIMEDIV)*3;
			theMatrix[curRow][i*4+j++] = (((double)deltaT)/TIMEDIV)*2;
			theMatrix[curRow][i*4+j++] = 1;
			theMatrix[curRow][i*4+j++] = 0;
			theMatrix[curRow][i*4+j++] = 0;//-(((double)deltaT)/TIMEDIV)*(((double)deltaT)/TIMEDIV)*3;
			theMatrix[curRow][i*4+j++] = 0;//-(((double)deltaT)/TIMEDIV)*2;
			theMatrix[curRow][i*4+j++] = -1;
			theMatrix[curRow][i*4+j++] = 0;
			theMatrix[curRow++][(curCam-1)*4+4] = 0;
			
			j=0;
			//second derivitives are equal
			theMatrix[curRow][i*4+j++] = (((double)deltaT)/TIMEDIV)*6;
			theMatrix[curRow][i*4+j++] = 2;
			theMatrix[curRow][i*4+j++] = 0;
			theMatrix[curRow][i*4+j++] = 0;
			theMatrix[curRow][i*4+j++] = 0;//-(((double)deltaT)/TIMEDIV)*6;
			theMatrix[curRow][i*4+j++] = -2;
			theMatrix[curRow][i*4+j++] = 0;
			theMatrix[curRow][i*4+j++] = 0;
			theMatrix[curRow++][(curCam-1)*4+4] = 0;
		}
		//4 more equations are needed to have a solvable matrix (there should be n-1 splines)
		
		j=0;
		//second derivitive at the end is zero
		theMatrix[curRow][(curCam-1)*4+j++] = (((double)cam[curCam].time-cam[curCam-1].time)/TIMEDIV)*6;
		theMatrix[curRow][(curCam-1)*4+j++] = 2;
		theMatrix[curRow][(curCam-1)*4+j++] = 0;
		theMatrix[curRow][(curCam-1)*4+j++] = 0;
		theMatrix[curRow++][(curCam-1)*4+4] = 0;
		
		j=0;
		//second derivitive at the beginning is zero
		theMatrix[curRow][j++] = 0;//(((double)cam[0].time)/TIMEDIV)*6;
		theMatrix[curRow][j++] = 2;
		theMatrix[curRow][j++] = 0;
		theMatrix[curRow][j++] = 0;
		theMatrix[curRow++][(curCam-1)*4+4] = 0;
		
		j=0;
		//runs through first point
		theMatrix[curRow][(curCam-1)*4+j++] = 0;//(((double)cam[curCam-1].time)/TIMEDIV)*(((double)cam[curCam-1].time)/TIMEDIV)*(((double)cam[curCam-1].time)/TIMEDIV);
		theMatrix[curRow][(curCam-1)*4+j++] = 0;//(((double)cam[curCam-1].time)/TIMEDIV)*(((double)cam[curCam-1].time)/TIMEDIV);
		theMatrix[curRow][(curCam-1)*4+j++] = 0;//(((double)cam[curCam-1].time)/TIMEDIV);
		theMatrix[curRow][(curCam-1)*4+j++] = 1;
		theMatrix[curRow++][(curCam-1)*4+4] = 0;//cam[(curCam-1)].origin[k];
		
		deltaT = (double)(cam[curCam].time - cam[curCam-1].time);
		j=0;
		//runs through second point
		//pretty sure that's supposed to say Last point... =/
		theMatrix[curRow][(curCam-1)*4+j++] = (((double)deltaT)/TIMEDIV)*(((double)deltaT)/TIMEDIV)*(((double)deltaT)/TIMEDIV);
		theMatrix[curRow][(curCam-1)*4+j++] = (((double)deltaT)/TIMEDIV)*(((double)deltaT)/TIMEDIV);
		theMatrix[curRow][(curCam-1)*4+j++] = (((double)deltaT)/TIMEDIV);
		theMatrix[curRow][(curCam-1)*4+j++] = 1;
		theMatrix[curRow++][(curCam-1)*4+4] = cam[curCam].origin[k] - cam[curCam-1].origin[k];
		
		numRows = curRow;
		numCols = curCam*4+1;
		rowReduce();
		
		for( i=0; i<curCam; i++ )
		{
			cam[i].csi_coeff[k][0] = theMatrix[i*4][curCam*4];
			cam[i].csi_coeff[k][1] = theMatrix[i*4+1][curCam*4];
			cam[i].csi_coeff[k][2] = theMatrix[i*4+2][curCam*4];
			cam[i].csi_coeff[k][3] = theMatrix[i*4+3][curCam*4];
		}
	}
	
	//yeah, i know, bad coding to copy/paste the whole loop, but we need to do it over again for angles and i
	//really dont want to go to using pointers and shit, would take a lot of recoding whereas this
	//already works..... (aka, i lazy :oops)
	
	for( k=0; k<3; k++ )
	{
		double deltaT;
		curRow = 0;
		memset( theMatrix[0], 0, sizeof( theMatrix ) );
		for( i=0; i<curCam-1; i++ )
		{
			double deltaT = (double)(cam[i+1].time - cam[i].time);
			double deltaD = (double)(cam[i+1].angle[k] - cam[i].angle[k]);
			j=0;
			//runs through first point
			//guess what? ive decided it's (0,0) kthx.
			theMatrix[curRow][i*4+j++] = 0;//(((double)cam[i].time)/TIMEDIV)*(((double)cam[i].time)/TIMEDIV)*(((double)cam[i].time)/TIMEDIV);
			theMatrix[curRow][i*4+j++] = 0;//(((double)cam[i].time)/TIMEDIV)*(((double)cam[i].time)/TIMEDIV);
			theMatrix[curRow][i*4+j++] = 0;//(((double)cam[i].time)/TIMEDIV);
			theMatrix[curRow][i*4+j++] = 1;
			theMatrix[curRow++][(curCam-1)*4+4] = 0;//cam[i].origin[k];
			
			j=0;
			//runs through second point
			//doodz we're makin dese thangs unit doodz so it b leeter :>
			theMatrix[curRow][i*4+j++] = (((double)deltaT)/TIMEDIV)*(((double)deltaT)/TIMEDIV)*(((double)deltaT)/TIMEDIV);
			theMatrix[curRow][i*4+j++] = (((double)deltaT)/TIMEDIV)*(((double)deltaT)/TIMEDIV);
			theMatrix[curRow][i*4+j++] = (((double)deltaT)/TIMEDIV);
			theMatrix[curRow][i*4+j++] = 1;
			theMatrix[curRow++][(curCam-1)*4+4] = deltaD;
			
			j=0;
			//first derivitives are equal
			theMatrix[curRow][i*4+j++] = (((double)deltaT)/TIMEDIV)*(((double)deltaT)/TIMEDIV)*3;
			theMatrix[curRow][i*4+j++] = (((double)deltaT)/TIMEDIV)*2;
			theMatrix[curRow][i*4+j++] = 1;
			theMatrix[curRow][i*4+j++] = 0;
			theMatrix[curRow][i*4+j++] = 0;//-(((double)deltaT)/TIMEDIV)*(((double)deltaT)/TIMEDIV)*3;
			theMatrix[curRow][i*4+j++] = 0;//-(((double)deltaT)/TIMEDIV)*2;
			theMatrix[curRow][i*4+j++] = -1;
			theMatrix[curRow][i*4+j++] = 0;
			theMatrix[curRow++][(curCam-1)*4+4] = 0;
			
			j=0;
			//second derivitives are equal
			theMatrix[curRow][i*4+j++] = (((double)deltaT)/TIMEDIV)*6;
			theMatrix[curRow][i*4+j++] = 2;
			theMatrix[curRow][i*4+j++] = 0;
			theMatrix[curRow][i*4+j++] = 0;
			theMatrix[curRow][i*4+j++] = 0;//-(((double)deltaT)/TIMEDIV)*6;
			theMatrix[curRow][i*4+j++] = -2;
			theMatrix[curRow][i*4+j++] = 0;
			theMatrix[curRow][i*4+j++] = 0;
			theMatrix[curRow++][(curCam-1)*4+4] = 0;
		}
		//4 more equations are needed to have a solvable matrix (there should be n-1 splines)
		
		j=0;
		//second derivitive at the end is zero
		theMatrix[curRow][(curCam-1)*4+j++] = (((double)cam[curCam].time-cam[curCam-1].time)/TIMEDIV)*6;
		theMatrix[curRow][(curCam-1)*4+j++] = 2;
		theMatrix[curRow][(curCam-1)*4+j++] = 0;
		theMatrix[curRow][(curCam-1)*4+j++] = 0;
		theMatrix[curRow++][(curCam-1)*4+4] = 0;
		
		j=0;
		//second derivitive at the beginning is zero
		theMatrix[curRow][j++] = 0;//(((double)cam[0].time)/TIMEDIV)*6;
		theMatrix[curRow][j++] = 2;
		theMatrix[curRow][j++] = 0;
		theMatrix[curRow][j++] = 0;
		theMatrix[curRow++][(curCam-1)*4+4] = 0;
		
		j=0;
		//runs through first point
		theMatrix[curRow][(curCam-1)*4+j++] = 0;//(((double)cam[curCam-1].time)/TIMEDIV)*(((double)cam[curCam-1].time)/TIMEDIV)*(((double)cam[curCam-1].time)/TIMEDIV);
		theMatrix[curRow][(curCam-1)*4+j++] = 0;//(((double)cam[curCam-1].time)/TIMEDIV)*(((double)cam[curCam-1].time)/TIMEDIV);
		theMatrix[curRow][(curCam-1)*4+j++] = 0;//(((double)cam[curCam-1].time)/TIMEDIV);
		theMatrix[curRow][(curCam-1)*4+j++] = 1;
		theMatrix[curRow++][(curCam-1)*4+4] = 0;//cam[(curCam-1)].origin[k];
		
		deltaT = (double)(cam[curCam].time - cam[curCam-1].time);
		j=0;
		//runs through second point
		theMatrix[curRow][(curCam-1)*4+j++] = (((double)deltaT)/TIMEDIV)*(((double)deltaT)/TIMEDIV)*(((double)deltaT)/TIMEDIV);
		theMatrix[curRow][(curCam-1)*4+j++] = (((double)deltaT)/TIMEDIV)*(((double)deltaT)/TIMEDIV);
		theMatrix[curRow][(curCam-1)*4+j++] = (((double)deltaT)/TIMEDIV);
		theMatrix[curRow][(curCam-1)*4+j++] = 1;
		theMatrix[curRow++][(curCam-1)*4+4] = cam[curCam].angle[k] - cam[curCam-1].angle[k];
		
		numRows = curRow;
		numCols = curCam*4+1;
		rowReduce();
		
		for( i=0; i<curCam; i++ )
		{
			cam[i].csi_coeff[k+3][0] = theMatrix[i*4][curCam*4];
			cam[i].csi_coeff[k+3][1] = theMatrix[i*4+1][curCam*4];
			cam[i].csi_coeff[k+3][2] = theMatrix[i*4+2][curCam*4];
			cam[i].csi_coeff[k+3][3] = theMatrix[i*4+3][curCam*4];
		}
	}
}

// end cubic spline interpolation code

static void CG_smoothcam_f(void)
{
	char x[20],y[20],z[20],time[20],*sec;
	int i=0;
	//if(!cg.demoPlayback)return;
	
	//origins
	trap_Argv( ++i, x, 20 );
	if(Q_stricmp(x,"TP")==0) {
		cam[curCam].thirdPerson = qtrue;
		++i;
	}
	trap_Argv( ++i, y, 20 );
	trap_Argv( ++i, z, 20 );
	VectorSet( cam[curCam].origin, atof(x), atof(y), atof(z) );
	
	//angles
	trap_Argv( ++i, x, 20 );
	trap_Argv( ++i, y, 20 );
	trap_Argv( ++i, z, 20 );
	VectorSet( cam[curCam].angle, atof(x), atof(y), atof(z) );
	
	trap_Argv( ++i, time, 20 );
	for( i=0; time[i]!=':' && time[i]; i++ );
	if(time[i]==0)
		sec = 0;
	else
	{
		time[i] = 0;
		sec = time+i+1;
	}
	cam[curCam].time = ( atoi( time ) * 60000 + atof( sec ) * 1000 );
	
	curCam++;

	reCalc();
	
	CG_Printf("Added Cam Location %d.\n",curCam);
}

static void CG_insertcam_f(void)
{
	char x[20],y[20],z[20],time[20],*sec;
	int i=0, insertCam, j;
	//if(!cg.demoPlayback)return;
	
	trap_Argv( ++i, x, 20 );
	insertCam = atoi( x );
	//push cams >= down one
	for ( j=curCam-1; j>=insertCam; j-- )
	{
		cam[ j+1 ] = cam[ j ];
	}
	//origins
	trap_Argv( ++i, x, 20 );
	if(Q_stricmp(x,"TP")==0) {
		cam[insertCam].thirdPerson = qtrue;
		++i;
	}
	trap_Argv( ++i, y, 20 );
	trap_Argv( ++i, z, 20 );
	VectorSet( cam[insertCam].origin, atof(x), atof(y), atof(z) );
	
	//angles
	trap_Argv( ++i, x, 20 );
	trap_Argv( ++i, y, 20 );
	trap_Argv( ++i, z, 20 );
	VectorSet( cam[insertCam].angle, atof(x), atof(y), atof(z) );
	
	trap_Argv( ++i, time, 20 );
	for( i=0; time[i]!=':' && time[i]; i++ );
	if(time[i]==0)
		sec = 0;
	else
	{
		time[i] = 0;
		sec = time+i+1;
	}
	cam[insertCam].time = ( atoi( time ) * 60000 + atof( sec ) * 1000 );
	
	curCam++;

	reCalc();
	
	CG_Printf("Inserted Cam Location %d.\n",insertCam);
}

static void CG_removecam_f(void)
{
	char x[20];
	int i=0, removeCam, j;
	//if(!cg.demoPlayback)return;
	
	if( trap_Argc() != 2 )
	{
		CG_Printf( "Usage: removecam <cam num>\n" );
		return;
	}
	
	trap_Argv( ++i, x, 20 );
	removeCam = atoi( x );
	//push cams >= up one
	for ( j=removeCam; j<curCam; j++ )
	{
		cam[ j ] = cam[ j+1 ];
	}
	
	curCam--;

	reCalc();
	
	CG_Printf("Removed Cam Location %d.\n",removeCam);
}

static void CG_removeallcams_f(void)
{
	int i;
	//if(!cg.demoPlayback)return;
	
	for ( i=0; i<curCam; i++ )
	{
		memset( &cam[ i ], 0, sizeof( cam[ i ] ) );
	}
	
	CG_Printf("Removed %d Cams.\n",curCam);
	curCam = 0;
}

static void CG_savecamcfg_f(void)
{
	char filename[MAX_QPATH];
	char cmdstring[MAX_STRING_CHARS];
	int i;
	fileHandle_t f;
	
	if (trap_Argc()!=2)
	{
		CG_Printf("Syntax: savecamcfg <filename>\n");
		return;
	}
	
	trap_Argv( 1, filename, sizeof( filename ) );
	
	trap_FS_FOpenFile( filename, &f, FS_APPEND_SYNC );
	if( !f )
	{
		CG_Printf( "Failed to open '%s' for writing.\n", filename );
		return;
	}
	
	CG_Printf( "Dumping Cam " );
	
	for( i=0; i<curCam; i++ )
	{
		CG_Printf("%d ",i);
		if( !cam[i].thirdPerson )
			Com_sprintf( cmdstring, sizeof( cmdstring ), "smoothcam %f %f %f %f %f %f %d:%06.3f\n",
						 cam[i].origin[0], cam[i].origin[1], cam[i].origin[2],
						 cam[i].angle[0], cam[i].angle[1], cam[i].angle[2],
						 cam[i].time / 60000, (float)cam[i].time / 1000 - cam[i].time / 60000 * 60 );
		else
			Com_sprintf( cmdstring, sizeof( cmdstring ), "smoothcam TP 0 %f %f %f %f %f %d:%06.3f\n",
						 cam[i].origin[1], cam[i].origin[2], cam[i].angle[0],
						 cam[i].angle[1], cam[i].angle[2], cam[i].time / 60000,
						 (float)cam[i].time / 1000 - cam[i].time / 60000 * 60 );
		trap_FS_Write( cmdstring, strlen( cmdstring ), f );
	}
	trap_FS_FCloseFile( f );
	CG_Printf( "\nCam Data Dumped to %s.\n", filename );
}

int showCam = -1;

static void CG_showcam_f(void)
{
	int i;
	
	if ( trap_Argc() > 1 )
	{
		char tmps[20];
		trap_Argv( 1, tmps, sizeof( tmps ) );
		showCam = i = atoi( tmps );
		if( !cam[i].thirdPerson )
			CG_Printf( "%d: %f %f %f %f %f %f %d:%06.3f\n", i,
						 cam[i].origin[0], cam[i].origin[1], cam[i].origin[2],
						 cam[i].angle[0], cam[i].angle[1], cam[i].angle[2],
						 cam[i].time / 60000, (float)cam[i].time / 1000 - cam[i].time / 60000 * 60 );
		else
			CG_Printf( "%d: TP 0 %f %f %f %f %f %d:%06.3f\n", i,
						 cam[i].origin[1], cam[i].origin[2], cam[i].angle[0],
						 cam[i].angle[1], cam[i].angle[2], cam[i].time / 60000,
						 (float)cam[i].time / 1000 - cam[i].time / 60000 * 60 );
		return;
	}
	
	
	for( i=0; i<curCam; i++ )
	{
		if( !cam[i].thirdPerson )
			CG_Printf( "%d: %f %f %f %f %f %f %d:%06.3f\n", i,
						 cam[i].origin[0], cam[i].origin[1], cam[i].origin[2],
						 cam[i].angle[0], cam[i].angle[1], cam[i].angle[2],
						 cam[i].time / 60000, (float)cam[i].time / 1000 - cam[i].time / 60000 * 60 );
		else
			CG_Printf( "%d: TP 0 %f %f %f %f %f %d:%06.3f\n", i,
						 cam[i].origin[1], cam[i].origin[2], cam[i].angle[0],
						 cam[i].angle[1], cam[i].angle[2], cam[i].time / 60000,
						 (float)cam[i].time / 1000 - cam[i].time / 60000 * 60 );
	}
}

void CG_DumpLoc_f( void )
{
	char locfilename[MAX_QPATH];
	char locationfile[MAX_STRING_CHARS];
	char location[MAX_STRING_CHARS];
	char time[20];
	char *extptr;//, *buffptr;
	fileHandle_t f;

	// Check for argument
	if (trap_Argc() < 3)
	{
		CG_Printf("Syntax: dumploc <start:time[.msec]> <filename> (<TP>)\n");
		return;
	}

	// parse the message
	trap_Argv( 2, location, sizeof(location) );
	trap_Argv( 1, time, 20 );

	// Open locationfile
	Q_strncpyz( locfilename, location, sizeof(locfilename) );
	Q_strncpyz( locationfile, location, sizeof(locationfile) );
	extptr = locfilename + strlen( locfilename ) - 4;
	trap_FS_FOpenFile( locfilename, &f, FS_APPEND_SYNC );
	if( !f )
	{
		CG_Printf( "Failed to open '%s' for writing.\n", locfilename );
		return;
	}

	// Build the entity definition
	if( trap_Argc() == 3)
		Com_sprintf( locationfile, sizeof(locationfile), "smoothcam %f %f %f %f %f %f %s\n",
					 cg.snap->ps.origin[0], cg.snap->ps.origin[1], cg.snap->ps.origin[2],
					 cg.snap->ps.viewangles[0], cg.snap->ps.viewangles[1], cg.snap->ps.viewangles[2],
					 time );
	else
		Com_sprintf( locationfile, sizeof(locationfile), "smoothcam TP 0 %f %f %f %f %f %s\n",
					 cg_thirdPersonHorzOffset.value, cg_thirdPersonVertOffset.value, cg_thirdPersonPitchOffset.value,
					 cg_thirdPersonAngle.value, cg_thirdPersonRange.value, time );

	// And write out/acknowledge
	trap_FS_Write( locationfile, strlen( locationfile ), f );
	trap_FS_FCloseFile( f );
	CG_Printf( "Cam Data Dumped to %s.\n", locfilename );
}

static void CG_AddLoc_f( void )
{
	char cmd[MAX_STRING_CHARS];
	char time[20];

	// Check for argument
	if (trap_Argc() < 2)
	{
		CG_Printf("Syntax: addloc <start:time[.msec]> (<TP>)\n");
		return;
	}

	// parse the message
	trap_Argv( 1, time, 20 );

	// Build the entity definition
	if( trap_Argc() == 2)
		Com_sprintf( cmd, sizeof(cmd), "smoothcam %f %f %f %f %f %f %s\n",
					 cg.snap->ps.origin[0], cg.snap->ps.origin[1], cg.snap->ps.origin[2],
					 cg.snap->ps.viewangles[0], cg.snap->ps.viewangles[1], cg.snap->ps.viewangles[2],
					 time );
	else
		Com_sprintf( cmd, sizeof(cmd), "smoothcam TP 0 %f %f %f %f %f %s\n",
					 cg_thirdPersonHorzOffset.value, cg_thirdPersonVertOffset.value, cg_thirdPersonPitchOffset.value,
					 cg_thirdPersonAngle.value, cg_thirdPersonRange.value, time );
	trap_SendConsoleCommand( cmd );
}

static void CG_InsLoc_f( void )
{
	char cmd[MAX_STRING_CHARS];
	char time[20];
	char camNum[20];

	// Check for argument
	if (trap_Argc() < 3)
	{
		CG_Printf("Syntax: insloc <cam num> <start:time[.msec]> (<TP>)\n");
		return;
	}

	// parse the message
	trap_Argv( 1, camNum, 20 );
	trap_Argv( 2, time, 20 );

	// Build the entity definition
	if( trap_Argc() == 3)
		Com_sprintf( cmd, sizeof(cmd), "insertcam %d %f %f %f %f %f %f %s\n", atoi( camNum ),
					 cg.snap->ps.origin[0], cg.snap->ps.origin[1], cg.snap->ps.origin[2],
					 cg.snap->ps.viewangles[0], cg.snap->ps.viewangles[1], cg.snap->ps.viewangles[2],
					 time );
	else
		Com_sprintf( cmd, sizeof(cmd), "insertcam %d TP 0 %f %f %f %f %f %s\n", atoi( camNum ),
					 cg_thirdPersonHorzOffset.value, cg_thirdPersonVertOffset.value, cg_thirdPersonPitchOffset.value,
					 cg_thirdPersonAngle.value, cg_thirdPersonRange.value, time );
	trap_SendConsoleCommand( cmd );
}

static void CG_ReplLoc_f( void )
{
	char cmd[MAX_STRING_CHARS];
	char time[20];
	char camNum[20];

	// Check for argument
	if (trap_Argc() < 3)
	{
		CG_Printf("Syntax: replloc <cam num> <start:time[.msec]> (<TP>)\n");
		return;
	}

	// parse the message
	trap_Argv( 1, camNum, 20 );
	trap_Argv( 2, time, 20 );

	// Build the entity definition
	if( trap_Argc() == 3)
		Com_sprintf( cmd, sizeof(cmd), "removecam %d;insertcam %d %f %f %f %f %f %f %s\n", atoi( camNum ), atoi( camNum ),
					 cg.snap->ps.origin[0], cg.snap->ps.origin[1], cg.snap->ps.origin[2],
					 cg.snap->ps.viewangles[0], cg.snap->ps.viewangles[1], cg.snap->ps.viewangles[2],
					 time );
	else
		Com_sprintf( cmd, sizeof(cmd), "removecam %d;insertcam %d TP 0 %f %f %f %f %f %s\n", atoi( camNum ), atoi( camNum ),
					 cg_thirdPersonHorzOffset.value, cg_thirdPersonVertOffset.value, cg_thirdPersonPitchOffset.value,
					 cg_thirdPersonAngle.value, cg_thirdPersonRange.value, time );
	trap_SendConsoleCommand( cmd );
}

static void CG_demospec_f()
{
	if(trap_Argc() == 1)
		CG_Printf("\"pug_demospec\" is:\"%d\" default:\"-1\"\n",pug_demospec.integer);
	else
	{
		char newv[MAX_STRING_CHARS];
		trap_Argv( 1, newv, sizeof( newv ) );
		pug_demospec.integer = atoi( newv );
	}
}

static void CG_RotateCam360_f()
{
	char camNumS[MAX_STRING_CHARS];
	if(trap_Argc() != 2)
	{
		CG_Printf("Usage: rotateCam360 <camnum>\n");
		return;
	}
	
	trap_Argv( 1, camNumS, sizeof( camNumS ) );
	cam[atoi(camNumS)].angle[1] += 360;
	
	// update cubic spline coefficients for new angle
	reCalc();
}

static void CG_RotateCamn360_f()
{
	char camNumS[MAX_STRING_CHARS];
	if(trap_Argc() != 2)
	{
		CG_Printf("Usage: rotateCam-360 <camnum>\n");
		return;
	}
	
	trap_Argv( 1, camNumS, sizeof( camNumS ) );
	cam[atoi(camNumS)].angle[1] -= 360;
	
	// update cubic spline coefficients for new angle
	reCalc();
}

static void CG_ListFiles_f()
{
	char path[2],extension[2],listbuf[MAX_STRING_CHARS];
	path[1] = 0;
	extension[0] = 0;
	trap_FS_GetFileList( path + 1, extension, listbuf, MAX_STRING_CHARS );
	CG_Printf("Files: %s\n",listbuf);
}

void _mkdir(char *);
/*
============
FS_CreatePath

Creates any directories needed to store the given filename
============
*/
static qboolean FS_CreatePath (char *OSPath) {
	char	*ofs;
	
	// make absolutely sure that it can't back up the path
	// FIXME: is c: allowed???
	if ( strstr( OSPath, ".." ) || strstr( OSPath, "::" ) ) {
		Com_Printf( "WARNING: refusing to create relative path \"%s\"\n", OSPath );
		return qtrue;
	}

	for (ofs = OSPath+1 ; *ofs ; ofs++) {
		if (*ofs == PATH_SEP) {	
			// create the directory
			*ofs = 0;
			_mkdir (OSPath);
			*ofs = PATH_SEP;
		}
	}
	return qfalse;
}

char *FS_BuildOSPath( const char *base, const char *game, const char *qpath );

FILE *ghostFile = 0;
qboolean ghostMode = qfalse;

static void CG_GhostSave_f()
{
	char file[MAX_STRING_CHARS];
	char prot[MAX_STRING_CHARS];
	char *testpath;
	if(trap_Argc() < 2) {
		CG_Printf("Filename required\n");
		return;
	}
	if(ghostFile) {
		CG_Printf("Error: Ghost already in use\n");
		return;
	}
	trap_Argv( 1, file, sizeof(file) );
	trap_Cvar_VariableStringBuffer( "protocol", prot, sizeof( prot ) );
	FS_CreatePath( "ghosts" );
	testpath = FS_BuildOSPath( CG_Cvar_String( "fs_homepath" ), CG_Cvar_String( "fs_game" ), va( "ghosts/%s.BOO_%s", file, prot ) );
	ghostFile = fopen( testpath, "wb" );
	ghostMode = qfalse;
}

static void CG_GhostLoad_f()
{
	char file[MAX_STRING_CHARS];
	char prot[MAX_STRING_CHARS];
	char *testpath;
	if(trap_Argc() < 2) {
		CG_Printf("Filename required\n");
		return;
	}
	if(ghostFile) {
		CG_Printf("Error: Ghost already in use\n");
		return;
	}
	trap_Argv( 1, file, sizeof(file) );
	trap_Cvar_VariableStringBuffer( "protocol", prot, sizeof( prot ) );
	testpath = FS_BuildOSPath( CG_Cvar_String( "fs_homepath" ), CG_Cvar_String( "fs_game" ), va( "ghosts/%s.BOO_%s", file, prot ) );
	ghostFile = fopen( testpath, "rb" );
	if (!ghostFile) {
		CG_Printf("Error: File not found\n" );
		return;
	}
	/*if( trap_FS_FOpenFile( va( "ghosts/%s.BOO_%s", file, prot ), &ghostFile, FS_READ ) == 0 )
	{
		ghostFile = -1;
		CG_Printf("Error: File not found\n" );
		return;
	}*/
	ghostMode = qtrue;
}

static void CG_GhostClose_f()
{
	if(ghostFile==0) {
		CG_Printf("Error: Ghost already closed\n");
		return;
	}
	fclose( ghostFile );
	ghostFile = 0;
}

static int Q_isdigit( char *c )
{
	int i;
	for( i=0; c[i]!=0; i++ )
		if (!((c[i] >= '0' && c[i] <= '9') || c[i] == '.'))
			return ( 0 );
	return ( 1 );
}

extern int trap_RealTime(qtime_t *qtime);

/***********************************

$viewposX - player view's X world position (same value as posX)
$viewposY - player view's Y world position (same value as posY)
$viewposZ - player view's Z world position (posZ + viewheight)
$viewposXYZ - player view's X Y Z world position

$posX - player's X world position
$posY - player's Y world position
$posZ - player's Z world position
$posXYZ - player's X Y Z world position

$angX - player's X viewangle
$angY - player's Y viewangle
$angZ - player's Z viewangle
$angXYZ - player's X Y Z viewangle

$velX - player's X velocity
$velY - player's Y velocity
$velZ - player's Z velocity
$velXYZ - player's X Y Z velocity

$speedXY - player's horizontal speed
$speedZ  - player's vertical speed

$mapname    - current map name
$date       - current date in YYYYMMDD format
$time       - current time in HHMMSSS format
$playername - player's name (colors removed)
$cgtime     - clientside timer

************************************/
static void CG_VarMath_f()
{
	int i;
	char varName[MAX_STRING_CHARS];
	char tmpBuf[MAX_STRING_CHARS],op;
	double curVal = 0;
	char curValS[MAX_STRING_CHARS];
	qboolean isString = qfalse;
	if(trap_Argc() < 2) {
		CG_Printf("\n^1varMath: ^2Performs math functions on cvars and vstrs.\n");
		CG_Printf("^7Usage:   varMath targetVariableName operator: +-*/%= value (+-*/%= value)\n");
		CG_Printf("^2Example: ^7varMath timescale * .2\n");
		CG_Printf("^2Example: ^7varMath user_num = 1 + 3 * 5\n");
		CG_Printf("^2Example: ^7varMath user_demoname = $mapname + _ + $date + _ + 001\n\n");
		CG_Printf("^3Note:    for immediate variable update, prefix the targetVariableName with \"user_\"\n");
		return;
	}

	if(trap_Argc() < 3) {
		CG_Printf( "^7*** Error - missing operator\n" );
		return;
	}

	if(trap_Argc() < 4) {
		CG_Printf( "^7*** Error - missing value\n" );
		return;
	}
	trap_Argv( 1, varName, sizeof( varName ) );
	trap_Cvar_VariableStringBuffer( varName, curValS, sizeof( curValS ) );
	curVal = atof( curValS );
	for( i=2; i<trap_Argc(); i+=2 )
	{
		trap_Argv( i, tmpBuf, sizeof( tmpBuf ) );
		op = tmpBuf[0];
		trap_Argv( i+1, tmpBuf, sizeof( tmpBuf ) );
		if( tmpBuf[0] == '$' )
		{
			if( !strcmp( tmpBuf+1, "viewposX" ) || !strcmp( tmpBuf+1, "posX" ) )
				Q_strncpyz( tmpBuf, va( "%f", pug_demospec.integer!=-1?cg_entities[pug_demospec.integer].lerpOrigin[0]:cg_entities[cg.snap->ps.clientNum].lerpOrigin[0] ), sizeof( tmpBuf ) );
			else if( !strcmp( tmpBuf+1, "viewposY" ) || !strcmp( tmpBuf+1, "posY" ) )
				Q_strncpyz( tmpBuf, va( "%f", pug_demospec.integer!=-1?cg_entities[pug_demospec.integer].lerpOrigin[1]:cg_entities[cg.snap->ps.clientNum].lerpOrigin[1] ), sizeof( tmpBuf ) );
			else if( !strcmp( tmpBuf+1, "viewposZ" ) )
				Q_strncpyz( tmpBuf, va( "%f", pug_demospec.integer!=-1?cg_entities[pug_demospec.integer].lerpOrigin[2]+DEFAULT_VIEWHEIGHT:cg_entities[cg.snap->ps.clientNum].lerpOrigin[2]+cg.snap->ps.viewheight ), sizeof( tmpBuf ) );
			else if( !strcmp( tmpBuf+1, "viewposXYZ" ) )
				Q_strncpyz( tmpBuf, va( "%f %f %f",
					pug_demospec.integer!=-1?cg_entities[pug_demospec.integer].lerpOrigin[0]:cg_entities[cg.snap->ps.clientNum].lerpOrigin[0],
					pug_demospec.integer!=-1?cg_entities[pug_demospec.integer].lerpOrigin[1]:cg_entities[cg.snap->ps.clientNum].lerpOrigin[1],
					pug_demospec.integer!=-1?cg_entities[pug_demospec.integer].lerpOrigin[2]+DEFAULT_VIEWHEIGHT:cg_entities[cg.snap->ps.clientNum].lerpOrigin[2]+cg.snap->ps.viewheight ), sizeof( tmpBuf ) );
			else if( !strcmp( tmpBuf+1, "posZ" ) )
				Q_strncpyz( tmpBuf, va( "%f", pug_demospec.integer!=-1?cg_entities[pug_demospec.integer].lerpOrigin[2]:cg_entities[cg.snap->ps.clientNum].lerpOrigin[2] ), sizeof( tmpBuf ) );
			else if( !strcmp( tmpBuf+1, "posXYZ" ) )
				Q_strncpyz( tmpBuf, va( "%f %f %f",
					pug_demospec.integer!=-1?cg_entities[pug_demospec.integer].lerpOrigin[0]:cg_entities[cg.snap->ps.clientNum].lerpOrigin[0],
					pug_demospec.integer!=-1?cg_entities[pug_demospec.integer].lerpOrigin[1]:cg_entities[cg.snap->ps.clientNum].lerpOrigin[1],
					pug_demospec.integer!=-1?cg_entities[pug_demospec.integer].lerpOrigin[2]:cg_entities[cg.snap->ps.clientNum].lerpOrigin[2] ), sizeof( tmpBuf ) );
			else if( !strcmp( tmpBuf+1, "angX" ) )
				Q_strncpyz( tmpBuf, va( "%f", pug_demospec.integer!=-1?cg_entities[pug_demospec.integer].lerpAngles[0]:cg_entities[cg.snap->ps.clientNum].lerpAngles[0] ), sizeof( tmpBuf ) );
			else if( !strcmp( tmpBuf+1, "angY" ) )
				Q_strncpyz( tmpBuf, va( "%f", pug_demospec.integer!=-1?cg_entities[pug_demospec.integer].lerpAngles[1]:cg_entities[cg.snap->ps.clientNum].lerpAngles[1] ), sizeof( tmpBuf ) );
			else if( !strcmp( tmpBuf+1, "angZ" ) )
				Q_strncpyz( tmpBuf, va( "%f", pug_demospec.integer!=-1?cg_entities[pug_demospec.integer].lerpAngles[2]:cg_entities[cg.snap->ps.clientNum].lerpAngles[2] ), sizeof( tmpBuf ) );
			else if( !strcmp( tmpBuf+1, "angXYZ" ) )
				Q_strncpyz( tmpBuf, va( "%f %f %f",
					pug_demospec.integer!=-1?cg_entities[pug_demospec.integer].lerpAngles[0]:cg_entities[cg.snap->ps.clientNum].lerpAngles[0],
					pug_demospec.integer!=-1?cg_entities[pug_demospec.integer].lerpAngles[1]:cg_entities[cg.snap->ps.clientNum].lerpAngles[1],
					pug_demospec.integer!=-1?cg_entities[pug_demospec.integer].lerpAngles[2]:cg_entities[cg.snap->ps.clientNum].lerpAngles[2] ), sizeof( tmpBuf ) );
			else if( !strcmp( tmpBuf+1, "velX" ) )
				Q_strncpyz( tmpBuf, va( "%f", pug_demospec.integer!=-1?cg_entities[pug_demospec.integer].playerState->velocity[0]:cg_entities[cg.snap->ps.clientNum].playerState->velocity[0] ), sizeof( tmpBuf ) );
			else if( !strcmp( tmpBuf+1, "velY" ) )
				Q_strncpyz( tmpBuf, va( "%f", pug_demospec.integer!=-1?cg_entities[pug_demospec.integer].playerState->velocity[1]:cg_entities[cg.snap->ps.clientNum].playerState->velocity[1] ), sizeof( tmpBuf ) );
			else if( !strcmp( tmpBuf+1, "velZ" ) || !strcmp( tmpBuf+1, "speedZ" ) )
				Q_strncpyz( tmpBuf, va( "%f", pug_demospec.integer!=-1?cg_entities[pug_demospec.integer].playerState->velocity[2]:cg_entities[cg.snap->ps.clientNum].playerState->velocity[2] ), sizeof( tmpBuf ) );
			else if( !strcmp( tmpBuf+1, "velXYZ" ) )
				Q_strncpyz( tmpBuf, va( "%f %f %f",
					pug_demospec.integer!=-1?cg_entities[pug_demospec.integer].playerState->velocity[0]:cg_entities[cg.snap->ps.clientNum].playerState->velocity[0],
					pug_demospec.integer!=-1?cg_entities[pug_demospec.integer].playerState->velocity[1]:cg_entities[cg.snap->ps.clientNum].playerState->velocity[1],
					pug_demospec.integer!=-1?cg_entities[pug_demospec.integer].playerState->velocity[2]:cg_entities[cg.snap->ps.clientNum].playerState->velocity[2] ), sizeof( tmpBuf ) );
			else if( !strcmp( tmpBuf+1, "speedXY" ) )
				Q_strncpyz( tmpBuf, va( "%f", pug_demospec.integer!=-1?
					sqrt(cg_entities[pug_demospec.integer].playerState->velocity[0] * cg_entities[pug_demospec.integer].playerState->velocity[0]+cg_entities[pug_demospec.integer].playerState->velocity[1] * cg_entities[pug_demospec.integer].playerState->velocity[1]):
					sqrt(cg_entities[cg.snap->ps.clientNum].playerState->velocity[0] * cg_entities[cg.snap->ps.clientNum].playerState->velocity[0]+cg_entities[cg.snap->ps.clientNum].playerState->velocity[1] * cg_entities[cg.snap->ps.clientNum].playerState->velocity[1]) ), sizeof( tmpBuf ) );
			else if( !strcmp( tmpBuf+1, "mapname" ) )
			{
				const char *pszServerInfo = CG_ConfigString(CS_SERVERINFO);
				Q_strncpyz( tmpBuf, COM_SkipPath(Info_ValueForKey(pszServerInfo, "mapname")), sizeof( tmpBuf ) );
				isString = qtrue;
			}
			else if( !strcmp( tmpBuf+1, "date" ) )
			{
				qtime_t ct;
				trap_RealTime(&ct);
				Q_strncpyz( tmpBuf, va( "%04d%02d%02d", ct.tm_year + 1900, ct.tm_mon+1,ct.tm_mday ), sizeof( tmpBuf ) );
				isString = qtrue;
			}
			else if( !strcmp( tmpBuf+1, "time" ) )
			{
				qtime_t ct;
				trap_RealTime(&ct);
				Q_strncpyz( tmpBuf, va( "%02d%02d%02d", ct.tm_hour, ct.tm_min, ct.tm_sec ), sizeof( tmpBuf ) );
				isString = qtrue;
			}
			else if( !strcmp( tmpBuf+1, "playername" ) )
			{
				Q_strncpyz( tmpBuf, cgs.clientinfo[ pug_demospec.integer!=-1?pug_demospec.integer:cg.snap->ps.clientNum ].name, sizeof( tmpBuf ) );
				isString = qtrue;
			}
			else if( !strcmp( tmpBuf+1, "cgtime" ) )
			{
				Q_strncpyz( tmpBuf, va( "%d" , cg.time ), sizeof( tmpBuf ) );
				isString = qtrue;
			}
			else
				trap_Cvar_VariableStringBuffer( tmpBuf+1, tmpBuf, sizeof( tmpBuf ) );
			if( tmpBuf[0] == 0 )tmpBuf[0]='$';
		}
		switch( op ) {
		case '+':
		{
			if( Q_isdigit( tmpBuf ) && !isString )
				curVal += atof( tmpBuf );
			else {
				Q_strcat( curValS, sizeof( curValS ), tmpBuf );
				isString = qtrue;
			}
			break;
		}
		case '-':
		{
			curVal -= atof( tmpBuf );
			break;
		}
		case '*':
		{
			curVal *= atof( tmpBuf );
			break;
		}
		case '/':
		{
			curVal /= atof( tmpBuf );
			break;
		}
		case '=':
		{
			if( Q_isdigit( tmpBuf ) && !isString )
				curVal = atof( tmpBuf );
			else {
				Q_strncpyz( curValS, tmpBuf, sizeof( curValS ) );
				isString = qtrue;
			}
			break;
		}
		case '%':
		{
			int tmpcurVal;
			tmpcurVal = (int)curVal;
			tmpcurVal %= atoi( tmpBuf );
			curVal = (double)tmpcurVal;
			break;
		}
		default:
		{
			CG_Printf("*** Parse error: invalid operator (%c)\n",op);
			return;
		}
		}
	}
	if( !strncmp( varName, "user_", strlen( "user_" ) ) )
		if( isString ) {
			trap_Cvar_Set( varName, curValS );
			CG_Printf( "^7%s ^4-->^7 %s\n", varName, curValS );
		} else if( curVal - (int)curVal ) {
			trap_Cvar_Set( varName, va( "%f", curVal ) );
			CG_Printf( "^7%s ^4-->^7 %f\n", varName, curVal );
		} else {
			trap_Cvar_Set( varName, va( "%d", (int)curVal ) );
			CG_Printf( "^7%s ^4-->^7 %d\n", varName, (int)curVal );
		}
	else
		if( isString ) {
			trap_SendConsoleCommand( va( "set %s %s", varName, curValS ) );
			CG_Printf( "^7%s ^4-->^7 %s\n", varName, curValS );
		} else if( curVal - (int)curVal ) {
			trap_SendConsoleCommand( va( "set %s %f", varName, curVal ) );
			CG_Printf( "^7%s ^4-->^7 %f\n", varName, curVal );
		} else {
			trap_SendConsoleCommand( va( "set %s %d", varName, (int)curVal ) );
			CG_Printf( "^7%s ^4-->^7 %d\n", varName, (int)curVal );
		}
}

static void CG_Rename_f()
{
	char newName[MAX_STRING_CHARS];
	int clNum;
	if( trap_Argc() < 3 ) return;
	trap_Argv( 1, newName, sizeof( newName ) );
	clNum = atoi( newName );
	trap_Argv( 2, newName, sizeof( newName ) );
	Q_strncpyz( cgs.clientinfo[clNum].name, newName, sizeof( cgs.clientinfo[clNum].name ) );
	cgs.clientinfo[clNum].name[ strlen( cgs.clientinfo[clNum].name ) + 1 ] = -1;
}

/*
====================
FS_ReplaceSeparators

Fix things up differently for win/unix/mac
====================
*/
static void FS_ReplaceSeparators( char *path ) {
	char	*s;

	for ( s = path ; *s ; s++ ) {
		if ( *s == '/' || *s == '\\' ) {
			*s = PATH_SEP;
		}
	}
}

/*
===================
FS_BuildOSPath

Qpath may have either forward or backwards slashes
===================
*/
char *FS_BuildOSPath( const char *base, const char *game, const char *qpath ) {
	char	temp[MAX_OSPATH];
	static char ospath[2][MAX_OSPATH];
	static int toggle;
	
	toggle ^= 1;		// flip-flop to allow two returns without clash

	if( !game || !game[0] ) {
		game = CG_Cvar_String( "fs_game" );
	}

	Com_sprintf( temp, sizeof(temp), "/%s/%s", game, qpath );
	FS_ReplaceSeparators( temp );	
	Com_sprintf( ospath[toggle], sizeof( ospath[0] ), "%s%s", base, temp );
	
	return ospath[toggle];
}

/*
================
FS_FileExists

Tests if the file exists in the current gamedir, this DOES NOT
search the paths.  This is to determine if opening a file to write
(which always goes into the current gamedir) will cause any overwrites.
NOTE TTimo: this goes with FS_FOpenFileWrite for opening the file afterwards
================
*/
qboolean FS_FileExists( const char *file )
{
	FILE *f;
	char *testpath;

	testpath = FS_BuildOSPath( CG_Cvar_String( "fs_homepath" ), CG_Cvar_String( "fs_game" ), file );

	f = fopen( testpath, "rb" );
	if (f) {
		fclose( f );
		return qtrue;
	}
	return qfalse;
}

#ifdef USE_PNG
#include "png.h"
#endif
/*int pngWrite( char *file_name, int width, int height, unsigned char *buffer )
{
	FILE *fp;
	png_structp png_ptr;
	png_infop info_ptr;
	//png_uint_32 width, height;
	int bit_depth, color_type, interlace_type;
	png_color_8 sig_bit;
	int i;
	int j;
	int k;
	int line;
	png_bytep *row_pointers;
	unsigned row;
	
	file_name = FS_BuildOSPath( CG_Cvar_String( "fs_homepath" ), CG_Cvar_String( "fs_game" ), file_name );
	//while( 1 );
	fp = fopen(file_name, "wb");
	png_ptr = png_create_write_struct( PNG_LIBPNG_VER_STRING,NULL,NULL,NULL );
	if( png_ptr == NULL )
	{
		fclose(fp);
		return 0;
	}
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL)
	{
		fclose(fp);
		png_destroy_write_struct(&png_ptr, NULL);
		return 0;
	}

	if (setjmp(png_ptr->jmpbuf))
	{
		png_destroy_write_struct(&png_ptr, &info_ptr);
		fclose(fp);
		return 0;
	}
	
	png_init_io(png_ptr, fp);
	//width = 64;
	//height = 64;
	bit_depth = 8; //8?
	color_type = PNG_COLOR_TYPE_RGB_ALPHA;
	
	png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, PNG_COLOR_TYPE_RGB_ALPHA,
      PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
		
	sig_bit.red = bit_depth;
	sig_bit.green = bit_depth;
	sig_bit.blue = bit_depth;
	sig_bit.alpha = bit_depth;
	png_set_sBIT(png_ptr, info_ptr, &sig_bit);
	
	png_write_info(png_ptr, info_ptr);
	png_set_packing(png_ptr);
	
	trap_TrueMalloc( (void **)&row_pointers, sizeof(png_bytep) * height);
	if( !row_pointers )
	{
		CG_Printf("out of memory!\n");
		return 0;
	}
	for (row = 0; row < height; row++)
	{
		if(color_type == PNG_COLOR_TYPE_RGB_ALPHA)
		{
			trap_TrueMalloc( (void **)&row_pointers[row], width * 4);
			if( !row_pointers[row] )
			{
				CG_Printf("out of memory!\n");
				return 0;
			}
		}
		else
		{
  			trap_TrueMalloc( (void **)&row_pointers[row], width * 3);
  			if( !row_pointers[row] )
			{
				CG_Printf("out of memory!\n");
				return 0;
			}
		}
	}
	
	if ( row_pointers == NULL )
	{
		return 0;
	}
	
	k = 0;
	i = 1;
	while( i <= height )
	{
		j = 0;
		while( j < width * 4 )
		{
			row_pointers[height-i][j+0] = buffer[k];
			k++;
			row_pointers[height-i][j+1] = buffer[k];
			k++;
			row_pointers[height-i][j+2] = buffer[k];
			k++;
			row_pointers[height-i][j+3] = 255;//buffer[k];
			//k++;
			
			//row_pointers[i][j+3] = (int)((float)(row_pointers[i][j+0] + row_pointers[i][j+1] + row_pointers[i][j+2])/3.0);
			//row_pointers[i][j+3] = 255;
			j += 4;
		}
		i++;
	}
	
	png_write_image(png_ptr, row_pointers);
	png_write_end(png_ptr, info_ptr);
	png_destroy_write_struct(&png_ptr, &info_ptr);
	fclose(fp);
	for (row = 0; row < height; row++)
		trap_TrueFree( (void **)&row_pointers[row] );
	trap_TrueFree( (void **)&row_pointers );
	return 1;
}*/

typedef struct {
	int						empty;
} trMME_t;

typedef enum {
	mmeShotFormatTGA,
	mmeShotFormatJPG,
	mmeShotFormatPNG,
	mmeShotFormatAVI,
} mmeShotFormat_t;

typedef enum {
	mmeShotTypeRGB,
	mmeShotTypeRGBA,
	mmeShotTypeGray,
} mmeShotType_t;

/*
============================================================================

MME additions

============================================================================
*/

#define TGAMASK 0xffffff
static int SaveTGA_RLERGB(byte *out, const int image_width, const int image_height, const void* image_buffer ) {
	int y;
	const unsigned int *inBuf = ( const unsigned int*)image_buffer;
	int dataSize = 0;
	
	for (y=0; y < image_height;y++) {
		int left = image_width;
		/* Prepare for the first block and write the first pixel */
		while ( left > 0 ) {
			/* Search for a block of similar pixels */
			int i, block = left > 128 ? 128 : left;
			unsigned int pixel = inBuf[0];
			/* Check for rle pixels */
			for ( i = 1;i < block;i++) {
				if ( inBuf[i] != pixel)
					break;
			}
			if ( i > 1  ) {
				out[dataSize++] = 0x80 | ( i - 1);
				out[dataSize++] = pixel >> 16;
				out[dataSize++] = pixel >> 8;
				out[dataSize++] = pixel >> 0;
			} else {
				int blockStart = dataSize++;
				/* Write some raw pixels no matter what*/
				out[dataSize++] = pixel >> 16;
				out[dataSize++] = pixel >> 8;
				out[dataSize++] = pixel >> 0;
				pixel = inBuf[1];
				for ( i = 1;i < block;i++) {
					if ( inBuf[i+1] == pixel)
						break;
					out[dataSize++] = pixel >> 16;
					out[dataSize++] = pixel >> 8;
					out[dataSize++] = pixel >> 0;
					pixel = inBuf[i+1];
				}
				out[blockStart] = i - 1;
			}
			inBuf += i;
			left -= i;
		}
	}
	return dataSize;
}

static int SaveTGA_RLEGray(byte *out, const int image_width, const int image_height, const void* image_buffer ) {
	int y;
	unsigned char *inBuf = (unsigned char*)image_buffer;

	int dataSize = 0;

	for (y=0; y < image_height;y++) {
		int left = image_width;
		int diffIndex, diff;
		unsigned char lastPixel, nextPixel;
		lastPixel = *inBuf++;

		diff = 0;
		while (left > 0 ) {
			int c, n;
			if (left >= 2) {
				nextPixel = *inBuf++;
				if (lastPixel == nextPixel) {
					if (diff) {
						out[diffIndex] = diff - 1;
						diff = 0;
					}
					left -= 2;
					c = left > 126 ? 126 : left;
					n = 0;

					while (c) {
						nextPixel = *inBuf++;
						if (lastPixel != nextPixel)
							break;
						c--; n++;
					}
					left -= n;
					out[dataSize++] = 0x80 | (n + 1);
					out[dataSize++] = lastPixel;
					lastPixel = nextPixel;
				} else {
finalDiff:
					left--;
					if (!diff) {
						diff = 1;
						diffIndex = dataSize++;
					} else if (++diff >= 128) {
						out[diffIndex] = diff - 1;
						diff = 0;
					}
					out[dataSize++] = lastPixel;
					lastPixel = nextPixel;
				}
			} else {
				goto finalDiff;
			}
		}
		if (diff) {
			out[diffIndex] = diff - 1;
		}
	}
	return dataSize;
}


/*
===============
SaveTGA
===============
*/
int SaveTGA( int image_compressed, int image_width, int image_height, mmeShotType_t image_type, byte *image_buffer, byte *out_buffer, int out_size ) {
	int i;
	int imagePixels = image_height * image_width;
	int pixelSize;
	int filesize = 18;	// header is here by default
	byte tgaFormat;

	// Fill in the header
	switch (image_type) {
	case mmeShotTypeGray:
		tgaFormat = 3;
		pixelSize = 1;
		break;
	case mmeShotTypeRGB:
		tgaFormat = 2;
		pixelSize = 3;
		break;
	default:
		return 0;
	}
	if (image_compressed)
		tgaFormat += 8;

	/* Clear the header */
	Com_Memset( out_buffer, 0, filesize );

	out_buffer[2] = tgaFormat;
	out_buffer[12] = image_width & 255;
	out_buffer[13] = image_width >> 8;
	out_buffer[14] = image_height & 255;
	out_buffer[15] = image_height >> 8;
	out_buffer[16] = 24;

	// Fill output buffer
	if (!image_compressed) { // Plain memcpy
		byte *buftemp = out_buffer+filesize;
		switch (image_type) {
		case mmeShotTypeRGB:
			for (i = 0; i < imagePixels; i++ ) {
				/* Also handle the RGB to BGR conversion here */
				*buftemp++ = image_buffer[2];
				*buftemp++ = image_buffer[1];
				*buftemp++ = image_buffer[0];
				image_buffer += 4;
			}
			filesize += image_width*image_height*3;
			break;
		case mmeShotTypeGray:
			/* Stupid copying of data here but oh well */
			Com_Memcpy( buftemp, image_buffer, image_width*image_height );
			filesize += image_width*image_height;
			break;
		}
	} else {
		switch (image_type) {
		case mmeShotTypeRGB:
			filesize += SaveTGA_RLERGB(out_buffer+filesize, image_width, image_height, image_buffer );
			break;
		case mmeShotTypeGray:
			filesize += SaveTGA_RLEGray(out_buffer+filesize, image_width, image_height, image_buffer );
			break;
		}
	}
	return filesize;
}

#ifdef USE_PNG
typedef struct {
	char *buffer;
	unsigned int bufferSize;
	unsigned int bufferUsed;
} PNGWriteData_t;

static void PNG_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
	PNGWriteData_t *ioData = (PNGWriteData_t *)png_get_io_ptr( png_ptr );
	if ( ioData->bufferUsed + length < ioData->bufferSize) {
		Com_Memcpy( ioData->buffer + ioData->bufferUsed, data, length );
		ioData->bufferUsed += length;
	}
}

static void PNG_flush_data(png_structp png_ptr) {

}

/* Save PNG */
int SavePNG( int compresslevel, int image_width, int image_height, mmeShotType_t image_type, byte *image_buffer, byte *out_buffer, int out_size ) {
	png_structp png_ptr = 0;
	png_infop info_ptr = 0;
	png_bytep *row_pointers = 0;
	PNGWriteData_t writeData;
	int i, rowSize;

	writeData.bufferUsed = 0;
	writeData.bufferSize = out_size;
	writeData.buffer = out_buffer;
	if (!writeData.buffer)
		goto skip_shot;
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL,NULL, NULL);
	if (!png_ptr)
		goto skip_shot;
	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
		goto skip_shot;

	/* Finalize the initing of png library */
    png_set_write_fn(png_ptr, &writeData, PNG_write_data, PNG_flush_data );
	if (compresslevel < 0 || compresslevel > Z_BEST_COMPRESSION)
		compresslevel = Z_DEFAULT_COMPRESSION;
	png_set_compression_level(png_ptr, compresslevel );
	
	/* set other zlib parameters */
	png_set_compression_mem_level(png_ptr, 5);
	png_set_compression_strategy(png_ptr,Z_DEFAULT_STRATEGY);
	png_set_compression_window_bits(png_ptr, 15);
	png_set_compression_method(png_ptr, 8);
	png_set_compression_buffer_size(png_ptr, 8192);
	if ( image_type == mmeShotTypeRGB ) {
		rowSize = image_width*4;
		png_set_IHDR(png_ptr, info_ptr, image_width, image_height, 8, 
			PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
		png_write_info(png_ptr, info_ptr );
		png_set_filler(png_ptr, 1, PNG_FILLER_AFTER);
	} else if ( image_type == mmeShotTypeGray ) {
		rowSize = image_width*1;
		png_set_IHDR(png_ptr, info_ptr, image_width, image_height, 8, 
			PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
		png_write_info(png_ptr, info_ptr );
	}
	/*Allocate an array of scanline pointers*/
	//row_pointers=(png_bytep*)malloc(image_height*sizeof(png_bytep));
	trap_TrueMalloc( (void **)&row_pointers, image_height*sizeof(png_bytep) );
	for (i=0;i<image_height;i++) {
		row_pointers[i]=(image_buffer+(image_height -1 - i )*rowSize );
	}
	/*tell the png library what to encode.*/
	png_write_image(png_ptr, row_pointers);
	png_write_end(png_ptr, 0);

	//PNG_TRANSFORM_PACKSWAP | PNG_TRANSFORM_STRIP_FILLER
skip_shot:
	if (png_ptr)
		png_destroy_write_struct(&png_ptr, &info_ptr);
	if (row_pointers)
		trap_TrueFree( (void **)&row_pointers );
	return writeData.bufferUsed;
}
#endif

//#include <gl/gl.h>

#define AVI_MAX_FRAMES	20000
#define AVI_MAX_SIZE	((2*1024-10)*1024*1024)
#define AVI_HEADER_SIZE	2048

typedef struct mmeAviFile_s {
	char name[MAX_OSPATH];
	FILE *file;
	float fps;
	int	width, height;
	int frames;
	int index[AVI_MAX_FRAMES];
	int	written;
	int format;
	mmeShotType_t type;
} mmeAviFile_t;

trMME_t trMME;

static char *workAlloc = 0;
static char *workAlign = 0;
static int workSize, workUsed;

#define BLURMAX 32
typedef struct {
	char name[MAX_OSPATH];
	int	 counter;
	mmeShotFormat_t format;
	mmeShotType_t type;
	mmeAviFile_t avi;
} mmeShot_t;

#include <xmmintrin.h>
static struct {
	int		pixelCount;
	__m64	*accumAlign;
	__m64	*overlapAlign;
	int		overlapTotal, overlapIndex;
	int		blurTotal, blurIndex;
	__m64	blurMultiply[256];
	struct {
		qboolean		take;
		float			fps;
		mmeShot_t		main, stencil, depth;
	} shot;
	qboolean allocFailed;
} shotData;

void R_MME_SaveShot( mmeShot_t *shot, int width, int height, float fps, qboolean gammaCorrect, byte *inBuf ) {
	mmeShotFormat_t format;
	char *extension;
	char *outBuf;
	int outSize;
	char fileName[MAX_OSPATH];

	// gamma correct
	if ( gammaCorrect ) {
		//R_GammaCorrect( inBuf, width * height * 4 );
	}
	format = shot->format;
	switch (format) {
	case mmeShotFormatJPG:
		extension = "jpg";
		break;
	case mmeShotFormatTGA:
		/* Seems hardly any program can handle grayscale tga, switching to png */
		if (shot->type == mmeShotTypeGray) {
			format = mmeShotFormatPNG;
			extension = "png";
		} else {
			extension = "tga";
		}
		break;
	case mmeShotFormatPNG:
		extension = "png";
		break;
	case mmeShotFormatAVI:
		//mmeAviShot( &shot->avi, shot->name, shot->type, width, height, fps, inBuf );
		return;
	}

	if (shot->counter < 0) {
		shot->counter = 0;
		while (shot->counter < 1000000000) {
			Com_sprintf( fileName, sizeof(fileName), "screenshots/%s.%010d.%s", shot->name, shot->counter, extension);
			if (!FS_FileExists( fileName ))
				break;
			shot->counter++;
		}
	}

	Com_sprintf( fileName,	sizeof(fileName), "screenshots/%s.%010d.%s", shot->name, shot->counter, extension );
	shot->counter++;

	outSize = width * height * 4;
	trap_TrueMalloc( (void **)&outBuf, outSize );
	switch ( format ) {
	/*case mmeShotFormatJPG:
		outSize = SaveJPG( mme_jpegQuality->integer, width, height, shot->type, inBuf, outBuf, outSize );
		break;*/
	case mmeShotFormatTGA:
		outSize = SaveTGA( mme_tgaCompression.integer, width, height, shot->type, inBuf, outBuf, outSize );
		break;
#ifdef USE_PNG
	case mmeShotFormatPNG:
		outSize = SavePNG( mme_pngCompression.integer, width, height, shot->type, inBuf, outBuf, outSize );
		break;
#endif
	default:
		outSize = 0;
	}
	if (outSize)
	{
		//ri.FS_WriteFile( fileName, outBuf, outSize );
		FILE *myFp;
		myFp = fopen( FS_BuildOSPath( CG_Cvar_String( "fs_homepath" ), CG_Cvar_String( "fs_game" ), fileName ), "wb" );
		fwrite( outBuf, outSize, 1, myFp );
		fclose( myFp );
	}
	trap_TrueFree( (void **)&outBuf );
}

static void accumCreateMultiply( void ) {
	float	blurBase[256];
	int		blurEntries[256];
	float	blurHalf = 0.5f * (shotData.blurTotal - 1 );
	float	strength = 128;
	float	bestStrength = 0;
	int		passes, bestTotal = 0;
	int		i;
	
	if (blurHalf <= 0)
		return;

	if (!Q_stricmp(mme_blurType.string, "gaussian")) {
		for (i = 0; i < shotData.blurTotal; i++) {
			double xVal = ((i - blurHalf) / blurHalf) * 3;
			double expVal = exp( - (xVal * xVal) / 2);
			double sqrtVal = 1.0f / sqrt( 2 * M_PI);
			blurBase[i] = sqrtVal * expVal;
		}
	} else if (!Q_stricmp(mme_blurType.string, "triangle")) {
		for (i = 0; i < shotData.blurTotal; i++) {
			if ( i <= blurHalf )
				blurBase[i] = 1 + i;
			else
				blurBase[i] = 1 + (shotData.blurTotal - 1 - i);
		}
	} else {
		int mulDelta = (256 << 10) / shotData.blurTotal;
		int mulIndex = 0;
		for (i = 0; i < shotData.blurTotal; i++) {
			int lastMul = mulIndex & ~((1 << 10) -1);
			mulIndex += mulDelta;
			blurBase[i] = (mulIndex - lastMul) >> 10;
		}
	}
	/* Check for best 256 match */
	for (passes = 32;passes >0;passes--) {
		int total = 0;
		for (i = 0; i < shotData.blurTotal; i++) 
			total += strength * blurBase[i];
		if (total > 256) {
			strength *= (256.0 / total);
		} else if (total < 256) {
			if ( total > bestTotal) {
				bestTotal = total;
				bestStrength = strength;
			}
			strength *= (256.0 / total); 
		} else {
			bestTotal = total;
			bestStrength = strength;
			break;
		}
	}
	for (i = 0; i < shotData.blurTotal; i++) {
		blurEntries[i] = bestStrength * blurBase[i];
	}
	for (i = 0; i < shotData.blurTotal; i++) {
		int a = blurEntries[i];
		shotData.blurMultiply[i] = _mm_set_pi16( a, a, a, a);
	}
	_mm_empty();
}

static void accumClearMultiply( __m64 *writer, const __m64 *reader, __m64 *multp, int count ) {
	 __m64 readVal, zeroVal, work0, work1, multiply;
	 int i;
	 multiply = *multp;
	 zeroVal = _mm_setzero_si64();
	 /* Add 2 pixels in a loop */
	 for (i = count / 2 ; i>0 ; i--) {
		 readVal = *reader++;
		 work0 = _mm_unpacklo_pi8( readVal, zeroVal );
		 work1 = _mm_unpackhi_pi8( readVal, zeroVal );
		 work0 = _mm_mullo_pi16( work0, multiply );
		 work1 = _mm_mullo_pi16( work1, multiply );
		 writer[0] = work0;
		 writer[1] = work1;
		 writer += 2;
	 }
	 _mm_empty();
}

static void accumAddMultiply( __m64 *writer, const __m64 *reader, __m64 *multp, int count ) {
	 __m64 readVal, zeroVal, work0, work1, multiply;
	 int i;
	 multiply = *multp;
	 zeroVal = _mm_setzero_si64();
	 /* Add 2 pixels in a loop */
	 for (i = count / 2 ; i>0 ; i--) {
		 readVal = *reader++;
		 work0 = _mm_unpacklo_pi8( readVal, zeroVal );
		 work1 = _mm_unpackhi_pi8( readVal, zeroVal );
		 work0 = _mm_mullo_pi16( work0, multiply );
		 work1 = _mm_mullo_pi16( work1, multiply );
		 writer[0] = _mm_add_pi16( writer[0], work0 );
		 writer[1] = _mm_add_pi16( writer[1], work1 );
		 writer += 2;
	 }
	 _mm_empty();
}

static void accumShift( const __m64 *reader,  __m64 *writer, int count ) {
	__m64 work0, work1, work2, work3;
	int i;
	/* Handle 4 pixels at once */
	for (i = count /4;i>0;i--) {
		work0 = _mm_srli_pi16 (reader[0], 8);
		work1 = _mm_srli_pi16 (reader[1], 8);
		work2 = _mm_srli_pi16 (reader[2], 8);
		work3 = _mm_srli_pi16 (reader[3], 8);
//		_mm_stream_pi( writer + 0, _mm_packs_pu16( work0, work1 ));
//		_mm_stream_pi( writer + 1, _mm_packs_pu16( work2, work3 ));
		writer[0] = _mm_packs_pu16( work0, work1 );
		writer[1] = _mm_packs_pu16( work2, work3 );
		writer+=2;
		reader+=4;
	}
	_mm_empty();
}

#define MME_STRING( s ) # s

void R_MME_CheckCvars( void ) {
	int pixelCount, newBlur;
	static int oldmbtc = -1;

	pixelCount = cgs.glconfig.vidHeight * cgs.glconfig.vidWidth;
	if (mme_blurType.modificationCount != oldmbtc) {
		oldmbtc = mme_blurType.modificationCount;
		accumCreateMultiply();
	}
	if (mme_blurFrames.integer > BLURMAX) {
		trap_Cvar_Set( "mme_blurFrames", MME_STRING( BLURMAX ));
	} else if (mme_blurFrames.integer < 0) {
		trap_Cvar_Set( "mme_blurFrames", "0" );
	}

	if (mme_blurOverlap.integer > BLURMAX ) {
		trap_Cvar_Set( "mme_blurOverlap", MME_STRING( BLURMAX ));
	} else if (mme_blurOverlap.integer < 0 ) {
		trap_Cvar_Set( "mme_blurOverlap", "0");
	}

	newBlur = mme_blurFrames.integer + mme_blurOverlap.integer ;

	if ((newBlur != shotData.blurTotal || pixelCount != shotData.pixelCount || shotData.overlapTotal != mme_blurOverlap.integer) 
		&& !shotData.allocFailed ) {
		workUsed = 0;

		shotData.blurTotal = newBlur;
		if ( newBlur ) {
			shotData.accumAlign = (__m64 *)(workAlign + workUsed);
			workUsed += pixelCount * sizeof( __m64 );
			workUsed = (workUsed + 7) & ~7;
			if ( workUsed > workSize) {
				CG_Printf("Failed to allocate %d bytes from the mme work buffer\n", workUsed );
				shotData.allocFailed = qtrue;
				goto alloc_Skip;
			}
			accumCreateMultiply();
		}
		shotData.overlapTotal = mme_blurOverlap.integer;
		if ( shotData.overlapTotal ) {
			shotData.overlapAlign = (__m64 *)(workAlign + workUsed);
			workUsed += shotData.overlapTotal * pixelCount * 4;
			workUsed = (workUsed + 7) & ~7;
			if ( workUsed > workSize) {
				CG_Printf("Failed to allocate %d bytes from the mme work buffer\n", workUsed );
				shotData.allocFailed = qtrue;
				goto alloc_Skip;
			}
		}
		shotData.overlapIndex = 0;
		shotData.blurIndex = 0;
	}
alloc_Skip:
	shotData.pixelCount = pixelCount;
}

float my_zFar = 0.0f;
float my_zNear = 0.0f;
extern void (WINAPI * trueglFrustum)(GLdouble,GLdouble,GLdouble,GLdouble,GLdouble,GLdouble);
extern void (WINAPI * trueglMatrixMode)( GLenum mode );
extern void (WINAPI * trueglLoadMatrixf)( const GLfloat *mat );

void WINAPI my_glFrustum(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)
{
	my_zFar = zFar;
	trueglFrustum( left, right, bottom, top, zNear, zFar );
}

GLenum my_mode = GL_MODELVIEW;

void WINAPI my_glMatrixMode( GLenum mode )
{
	my_mode = mode;
	trueglMatrixMode( mode );
}

void WINAPI my_glLoadMatrixf( const GLfloat *mat )
{
	float x = mat[10]; // = (-zFar - zNear) / (zFar - zNear)
	float y=  mat[14]; // = -2 * zFar * zNear / depth
	if( my_mode == GL_PROJECTION )
	{
		my_zFar = y / ( x + 1 );
		my_zNear = y / ( x - 1 );
	}
	trueglLoadMatrixf( mat );
}

void R_MME_TakeShot( void ) {
	byte *outAlloc;
	__m64 *outAlign;
	/*qboolean doGamma;*/

	if ( !shotData.shot.take )
		return;


	/*doGamma = ( mme_screenShotGamma->integer || (tr.overbrightBits > 0) ) && glConfig.deviceSupportsGamma;*/
	R_MME_CheckCvars();

	trap_TrueMalloc( (void **)&outAlloc, shotData.pixelCount * 4 + 8);
	outAlign = (__m64 *)((((int)(outAlloc))+7) & ~7);

	if (mme_saveShot.integer && shotData.blurTotal && !shotData.allocFailed) {
		if ( shotData.overlapTotal ) {
			int lapIndex = shotData.overlapIndex % shotData.overlapTotal;
			shotData.overlapIndex++;
			/* First frame in a sequence, fill the buffer with the last frames */
			if (shotData.blurIndex == 0) {
				int i, index;
				index = lapIndex;
				accumClearMultiply( shotData.accumAlign, shotData.overlapAlign + (index * shotData.pixelCount/2), shotData.blurMultiply + 0, shotData.pixelCount );
				for (i = 1; i < shotData.overlapTotal; i++) {
					index = (index + 1 ) % shotData.overlapTotal;
					accumAddMultiply( shotData.accumAlign, shotData.overlapAlign + (index * shotData.pixelCount/2), shotData.blurMultiply + i, shotData.pixelCount );
				}
				shotData.blurIndex = shotData.overlapTotal;
			}
			glReadPixels( 0, 0, cgs.glconfig.vidWidth, cgs.glconfig.vidHeight, GL_RGBA, GL_UNSIGNED_BYTE, (byte *)(shotData.overlapAlign + (lapIndex * shotData.pixelCount/2)) ); 
			/*if ( doGamma && mme_blurGamma->integer ) {
				R_GammaCorrect( (byte *)outAlign, glConfig.vidWidth * glConfig.vidHeight * 4 );
			}*/
			accumAddMultiply( shotData.accumAlign, shotData.overlapAlign + (lapIndex * shotData.pixelCount/2), shotData.blurMultiply + shotData.blurIndex, shotData.pixelCount );
			shotData.blurIndex++;
		} else {
			glReadPixels( 0, 0, cgs.glconfig.vidWidth, cgs.glconfig.vidHeight, GL_RGBA, GL_UNSIGNED_BYTE, (byte *)outAlign ); 
			/*if ( doGamma && mme_blurGamma->integer ) {
				R_GammaCorrect( (byte *) outAlign, glConfig.vidWidth * glConfig.vidHeight * 4 );
			}*/
			if (shotData.blurIndex == 0) {
				accumClearMultiply( shotData.accumAlign, outAlign, shotData.blurMultiply + 0, shotData.pixelCount );
			} else {
				accumAddMultiply( shotData.accumAlign, outAlign, shotData.blurMultiply + shotData.blurIndex, shotData.pixelCount );
			}
			shotData.blurIndex++;
		}

		if (shotData.blurIndex >= shotData.blurTotal) {
			shotData.blurIndex = 0;
			accumShift( shotData.accumAlign, outAlign, shotData.pixelCount );
			R_MME_SaveShot( &shotData.shot.main, cgs.glconfig.vidWidth, cgs.glconfig.vidHeight, shotData.shot.fps, qfalse /*doGamma && !mme_blurGamma->integer*/, (byte *)outAlign );
		}
	} else {
		if (mme_saveStencil.integer) {
			glReadPixels( 0, 0, cgs.glconfig.vidWidth, cgs.glconfig.vidHeight, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, outAlign ); 
			R_MME_SaveShot( &shotData.shot.stencil, cgs.glconfig.vidWidth, cgs.glconfig.vidHeight, shotData.shot.fps, 0, (byte *)outAlign );
		}
		if ( mme_saveDepth.integer && mme_depthRange.value > 0 ) {
			float focusStart, focusEnd, focusMul;
			float zBase, zAdd, zRange;
			int i;
			float zFar, zNear;
			float zMax = 0.0f;

			focusStart = mme_depthFocus.value - mme_depthRange.value;
			focusEnd = mme_depthFocus.value + mme_depthRange.value;
			focusMul = 255.0f / (2 * mme_depthRange.value);

			glDepthRange( 0.0f, 1.0f );
			glReadPixels( 0, 0, cgs.glconfig.vidWidth, cgs.glconfig.vidHeight, GL_DEPTH_COMPONENT, GL_FLOAT, outAlign ); 
			
			
//			trap_R_GetDistanceCull( &zFar );
			zFar = my_zFar;
			zNear = my_zNear;//CG_Cvar_Get( "r_znear" );
			
			zRange = zFar - zNear;
            zBase = ( zFar + zNear ) / zRange;
			zAdd =  ( 2 * zFar * zNear ) / zRange;

//			CG_Printf("zFar: %f     my_zFar: %f\n", zFar, my_zFar);
//			CG_Printf("zNear: %f     my_zNear: %f\n", zNear, my_zNear);
			
			/* Could probably speed this up a bit with SSE but frack it for now */
			for (i=0;i<shotData.pixelCount;i++) {
				int outVal;
				/* Read from the 0 - 1 depth */
				float zVal = ((float *)outAlign)[i];
				if( zVal > zMax ) zMax = zVal;
				
				/* Back to the original -1 to 1 range */
				zVal = zVal * 2.0f - 1.0f;
				/* Back to the original z values */
				zVal = zAdd / ( zBase - zVal );
				/* Clip and scale the range that's been selected */
				if (zVal <= focusStart)
					outVal = 0;
				else if (zVal >= focusEnd)
					outVal = 255;
				else 
					outVal = (zVal - focusStart) * focusMul;
				((byte *)outAlign)[i] = outVal;
			}
			R_MME_SaveShot( &shotData.shot.depth, cgs.glconfig.vidWidth, cgs.glconfig.vidHeight, shotData.shot.fps, 0, (byte *)outAlign );
		}
		if ( mme_saveShot.integer ) {
			glReadPixels( 0, 0, cgs.glconfig.vidWidth, cgs.glconfig.vidHeight, GL_RGBA, GL_UNSIGNED_BYTE, outAlign ); 
			R_MME_SaveShot( &shotData.shot.main, cgs.glconfig.vidWidth, cgs.glconfig.vidHeight, shotData.shot.fps, qfalse/*doGamma*/, (byte *)outAlign );
		}

	}
	trap_TrueFree( (void **)&outAlloc );
	shotData.shot.take = qfalse;
}

typedef struct {
	int		commandId;
	char	name[MAX_QPATH];
	float	fps;
} captureCommand_t;

void R_MME_CaptureShotCmd( const void *data ) {
	const captureCommand_t *cmd = (const captureCommand_t *)data;
	static int mssfmc = -1;

	if (!cmd->name[0])
		return;// (const void *)(cmd + 1);

	shotData.shot.take = qtrue;
	shotData.shot.fps = cmd->fps;
	if (strcmp( cmd->name, shotData.shot.main.name) || mme_screenShotFormat.modificationCount != mssfmc ) {
		/* Also clear the buffer surfaces */
		shotData.blurIndex = 0;
		if ( workAlign )
			Com_Memset( workAlign, 0, workUsed );
		Com_sprintf( shotData.shot.main.name, sizeof( shotData.shot.main.name ), "%s", cmd->name );
		Com_sprintf( shotData.shot.depth.name, sizeof( shotData.shot.depth.name ), "%s.depth", cmd->name );
		Com_sprintf( shotData.shot.stencil.name, sizeof( shotData.shot.stencil.name ), "%s.stencil", cmd->name );
		
		mssfmc = mme_screenShotFormat.modificationCount;

		if (!Q_stricmp(mme_screenShotFormat.string, "jpg")) {
			shotData.shot.main.format = mmeShotFormatJPG;
		} else if (!Q_stricmp(mme_screenShotFormat.string, "tga")) {
			shotData.shot.main.format = mmeShotFormatTGA;
		} else if (!Q_stricmp(mme_screenShotFormat.string, "png")) {
			shotData.shot.main.format = mmeShotFormatPNG;
		} else if (!Q_stricmp(mme_screenShotFormat.string, "avi")) {
			shotData.shot.main.format = mmeShotFormatAVI;
		} else {
			shotData.shot.main.format = mmeShotFormatTGA;
		}
		
//		if (shotData.shot.main.format != mmeShotFormatAVI) {
			shotData.shot.depth.format = mmeShotFormatPNG;
			shotData.shot.stencil.format = mmeShotFormatPNG;
//		} else {
//			shotData.shot.depth.format = mmeShotFormatAVI;
//			shotData.shot.stencil.format = mmeShotFormatAVI;
//		}

		shotData.shot.main.type = mmeShotTypeRGB;
		shotData.shot.main.counter = -1;
		shotData.shot.depth.type = mmeShotTypeGray;
		shotData.shot.depth.counter = -1;
		shotData.shot.stencil.type = mmeShotTypeGray;
		shotData.shot.stencil.counter = -1;	
	}
	return;// (const void *)(cmd + 1);	
}

void R_MME_Init(void)
{
	if (!workAlloc) {
		workSize = mme_workMegs.integer;
		if (workSize < 64)
			workSize = 64;
		if (workSize > 512)
			workSize = 512;
		workSize *= 1024 * 1024;
		trap_TrueMalloc( (void **)&workAlloc, workSize + 16 );
		if (!workAlloc) {
			CG_Printf("Failed to allocate %d bytes for mme work buffer\n", workSize );
			return;
		}
		Com_Memset( workAlloc, 0, workSize + 16 );
		workAlign = (char *)(((int)workAlloc + 15) & ~15);
	}
}

void R_MME_Shutdown(void)
{
	if (workAlloc) {
		trap_TrueFree( (void **)&workAlloc );
	}
}

void R_Screenshot_PNG(int x, int y, int width, int height, char *fileName)
{
	static int curCount = 0;
	int i;
	static byte *buffer;
	captureCommand_t myCmd;
	myCmd.commandId = 0;
	Q_strncpyz( myCmd.name, mme_screenshotName.string, sizeof( myCmd.name ) );
	myCmd.fps = cg_blendScreenshots.integer;
	R_MME_CaptureShotCmd( (const void *)&myCmd );
	R_MME_TakeShot();
	
	/*if( cg_blendScreenshots.integer )
	{
		int factor = CG_Cvar_Int( "cl_avidemo" ) / cg_blendScreenshots.integer;
		byte *tmpBuffer;
		// read / average stuff
		if( !buffer )
		{
			trap_TrueMalloc( (void **)&buffer, cgs.glconfig.vidWidth*cgs.glconfig.vidHeight*3 );
			memset( buffer, 0, cgs.glconfig.vidWidth*cgs.glconfig.vidHeight*3 );
		}
		trap_TrueMalloc( (void **)&tmpBuffer, cgs.glconfig.vidWidth*cgs.glconfig.vidHeight*3 );
		if( !buffer || !tmpBuffer )
		{
			CG_Printf("out of memory!\n");
			return;
		}
		glReadPixels( x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE, tmpBuffer );
		for( i=0; i<cgs.glconfig.vidWidth*cgs.glconfig.vidHeight*3; i++ )
		{
			buffer[i] = ( buffer[i] * curCount / ( curCount + 1 ) ) + ( tmpBuffer[i] / ( curCount + 1 ) );
		}
		curCount++;
		if( curCount >= factor )
		{
			pngWrite( fileName, width, height, (unsigned char *)buffer );
			trap_TrueFree( (void **)&buffer );
			buffer = curCount = 0;
		}
		trap_TrueFree( (void **)&tmpBuffer );
	} else {
		trap_TrueMalloc( (void **)&buffer, cgs.glconfig.vidWidth*cgs.glconfig.vidHeight*3 );
		glReadPixels( x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer );
		pngWrite( fileName, width, height, (unsigned char *)buffer );
		trap_TrueFree( (void **)&buffer );
	}*/
}

/* 
================== 
R_ScreenshotFilename
================== 
*/  
void R_ScreenshotFilename( int lastNumber, char *fileName ) {
	int		a,b,c,d;

	if ( lastNumber < 0 || lastNumber > 9999 ) {
		Com_sprintf( fileName, MAX_OSPATH, "screenshots/shot9999.png" );
		return;
	}

	a = lastNumber / 1000;
	lastNumber -= a*1000;
	b = lastNumber / 100;
	lastNumber -= b*100;
	c = lastNumber / 10;
	lastNumber -= c*10;
	d = lastNumber;

	Com_sprintf( fileName, MAX_OSPATH, "screenshots/shot%i%i%i%i.png"
		, a, b, c, d );
}

/* 
================== 
R_ScreenShot_f

screenshot
screenshot [silent]
screenshot [levelshot]
screenshot [filename]

Doesn't print the pacifier message if there is a second arg
================== 
*/  
void R_ScreenShot_f (void) {
	char	checkname[MAX_OSPATH],tmp[MAX_STRING_CHARS];
	static	int	lastNumber = -1;
	qboolean	silent;

	/*if ( !strcmp( trap_Argv(1), "levelshot" ) ) {
		R_LevelShot();
		return;
	}*/
	
	trap_Argv( 1, tmp, sizeof( tmp ) );

	if ( !strcmp( tmp, "silent" ) ) {
		silent = qtrue;
	} else {
		silent = qfalse;
	}

	/*if ( trap_Argc() == 2 && !silent ) {
		// explicit filename
		Com_sprintf( checkname, MAX_OSPATH, "screenshots/%s.png", tmp );
	} else {
		// scan for a free filename

		// if we have saved a previous screenshot, don't scan
		// again, because recording demo avis can involve
		// thousands of shots
		if ( lastNumber == -1 ) {
			lastNumber = 0;
		}
		// scan for a free number
		for ( ; lastNumber <= 9999 ; lastNumber++ ) {
			R_ScreenshotFilename( lastNumber, checkname );
		lastNumber--;

      if (!FS_FileExists( checkname ))
      {
        break; // file doesn't exist
      }
		}

		if ( lastNumber >= 9999 ) {
			CG_Printf ("ScreenShot: Couldn't create a file\n"); 
			return;
 		}

		lastNumber++;
	}*/

	R_Screenshot_PNG( 0, 0, cgs.glconfig.vidWidth, cgs.glconfig.vidHeight, checkname );

	/*if ( !silent ) {
		CG_Printf ("Wrote %s\n", checkname);
	}*/
}

extern int camPreviewTime;
static void CG_PreviewCam_f()
{
	camPreviewTime = cg.time;
	CG_Printf("Started previewing cam\n");
}

static void CG_StopPreviewCam_f()
{
	camPreviewTime = 0;
	CG_Printf("Stopped previewing cam\n");
}

typedef struct {
	char	*cmd;
	void	(*function)(void);
} consoleCommand_t;

static consoleCommand_t	commands[] = {
	{ "testgun", CG_TestGun_f },
	{ "testmodel", CG_TestModel_f },
	{ "nextframe", CG_TestModelNextFrame_f },
	{ "prevframe", CG_TestModelPrevFrame_f },
	{ "nextskin", CG_TestModelNextSkin_f },
	{ "prevskin", CG_TestModelPrevSkin_f },
	{ "viewpos", CG_Viewpos_f },
	{ "+scores", CG_ScoresDown_f },
	{ "-scores", CG_ScoresUp_f },
	{ "sizeup", CG_SizeUp_f },
	{ "sizedown", CG_SizeDown_f },
	{ "weapnext", CG_NextWeapon_f },
	{ "weapprev", CG_PrevWeapon_f },
	{ "weapon", CG_Weapon_f },
	{ "weaponclean", CG_WeaponClean_f },
	{ "tell_target", CG_TellTarget_f },
	{ "tell_attacker", CG_TellAttacker_f },
	{ "tcmd", CG_TargetCommand_f },
	{ "spWin", CG_spWin_f },
	{ "spLose", CG_spLose_f },
	{ "scoresDown", CG_scrollScoresDown_f },
	{ "scoresUp", CG_scrollScoresUp_f },
	{ "startOrbit", CG_StartOrbit_f },
	//{ "camera", CG_Camera_f },
	{ "loaddeferred", CG_LoadDeferredPlayers },
	{ "invnext", CG_NextInventory_f },
	{ "invprev", CG_PrevInventory_f },
	{ "forcenext", CG_NextForcePower_f },
	{ "forceprev", CG_PrevForcePower_f },
	{ "briefing", CG_SiegeBriefing_f },
	{ "siegeCvarUpdate", CG_SiegeCvarUpdate_f },
	{ "siegeCompleteCvarUpdate", CG_SiegeCompleteCvarUpdate_f },
    { "clientserverstatus", CG_clientserverstatus_f },
    { "demopos", CG_demopos_f },
    { "smoothcam", CG_smoothcam_f },
    { "insertcam", CG_insertcam_f },
    { "removecam", CG_removecam_f },
    { "removeallcams", CG_removeallcams_f },
    { "savecamcfg", CG_savecamcfg_f },
    { "showcam", CG_showcam_f },
	{ "dumploc", CG_DumpLoc_f },
	{ "addloc", CG_AddLoc_f },
	{ "insloc", CG_InsLoc_f },
	{ "replloc", CG_ReplLoc_f },
	{ "listfiles", CG_ListFiles_f },
	{ "truepug_demospec", CG_demospec_f },
	{ "ghostsave", CG_GhostSave_f },
	{ "ghostload", CG_GhostLoad_f },
	{ "ghostclose", CG_GhostClose_f },
	{ "varMath", CG_VarMath_f },
	{ "rename", CG_Rename_f },
	{ "screenshot_png", R_ScreenShot_f },
	{ "previewCam", CG_PreviewCam_f },
	{ "stopPreviewCam", CG_StopPreviewCam_f },
	{ "rotateCam360", CG_RotateCam360_f },
	{ "rotateCam-360", CG_RotateCamn360_f },
};


/*
=================
CG_ConsoleCommand

The string has been tokenized and can be retrieved with
Cmd_Argc() / Cmd_Argv()
=================
*/
qboolean CG_ConsoleCommand( void ) {
	const char	*cmd;
	int		i;

	cmd = CG_Argv(0);

	for ( i = 0 ; i < sizeof( commands ) / sizeof( commands[0] ) ; i++ ) {
		if ( !Q_stricmp( cmd, commands[i].cmd ) ) {
			commands[i].function();
			return qtrue;
		}
	}

	return qfalse;
}


/*
=================
CG_InitConsoleCommands

Let the client system know about all of our commands
so it can perform tab completion
=================
*/
void CG_InitConsoleCommands( void ) {
	int		i;

	for ( i = 0 ; i < sizeof( commands ) / sizeof( commands[0] ) ; i++ ) {
		trap_AddCommand( commands[i].cmd );
	}

	//
	// the game server will interpret these commands, which will be automatically
	// forwarded to the server after they are not recognized locally
	//
	trap_AddCommand ("forcechanged");
	trap_AddCommand ("sv_invnext");
	trap_AddCommand ("sv_invprev");
	trap_AddCommand ("sv_forcenext");
	trap_AddCommand ("sv_forceprev");
	trap_AddCommand ("sv_saberswitch");
	trap_AddCommand ("engage_duel");
	trap_AddCommand ("force_heal");
	trap_AddCommand ("force_speed");
	trap_AddCommand ("force_throw");
	trap_AddCommand ("force_pull");
	trap_AddCommand ("force_distract");
	trap_AddCommand ("force_rage");
	trap_AddCommand ("force_protect");
	trap_AddCommand ("force_absorb");
	trap_AddCommand ("force_healother");
	trap_AddCommand ("force_forcepowerother");
	trap_AddCommand ("force_seeing");
	trap_AddCommand ("use_seeker");
	trap_AddCommand ("use_field");
	trap_AddCommand ("use_bacta");
	trap_AddCommand ("use_electrobinoculars");
	trap_AddCommand ("zoom");
	trap_AddCommand ("use_sentry");
	trap_AddCommand ("bot_order");
	trap_AddCommand ("saberAttackCycle");
	trap_AddCommand ("kill");
	trap_AddCommand ("say");
	trap_AddCommand ("say_team");
	trap_AddCommand ("tell");
	trap_AddCommand ("give");
	trap_AddCommand ("god");
	trap_AddCommand ("notarget");
	trap_AddCommand ("noclip");
	trap_AddCommand ("team");
	trap_AddCommand ("follow");
	trap_AddCommand ("levelshot");
	trap_AddCommand ("addbot");
	trap_AddCommand ("setviewpos");
	trap_AddCommand ("callvote");
	trap_AddCommand ("vote");
	trap_AddCommand ("callteamvote");
	trap_AddCommand ("teamvote");
	trap_AddCommand ("stats");
	trap_AddCommand ("teamtask");
	trap_AddCommand ("loaddefered");	// spelled wrong, but not changing for demo
}
