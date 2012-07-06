// Copyright (C) 1999-2000 Id Software, Inc.
//
// cg_view.c -- setup all the parameters (position, angle, etc)
// for a 3D rendering
#include "cg_local.h"

#include "bg_saga.h"

#if !defined(CL_LIGHT_H_INC)
	#include "cg_lights.h"
#endif

#define MASK_CAMERACLIP (MASK_SOLID|CONTENTS_PLAYERCLIP)
#define CAMERA_SIZE	4


/*
=============================================================================

  MODEL TESTING

The viewthing and gun positioning tools from Q2 have been integrated and
enhanced into a single model testing facility.

Model viewing can begin with either "testmodel <modelname>" or "testgun <modelname>".

The names must be the full pathname after the basedir, like 
"models/weapons/v_launch/tris.md3" or "players/male/tris.md3"

Testmodel will create a fake entity 100 units in front of the current view
position, directly facing the viewer.  It will remain immobile, so you can
move around it to view it from different angles.

Testgun will cause the model to follow the player around and supress the real
view weapon model.  The default frame 0 of most guns is completely off screen,
so you will probably have to cycle a couple frames to see it.

"nextframe", "prevframe", "nextskin", and "prevskin" commands will change the
frame or skin of the testmodel.  These are bound to F5, F6, F7, and F8 in
q3default.cfg.

If a gun is being tested, the "gun_x", "gun_y", and "gun_z" variables will let
you adjust the positioning.

Note that none of the model testing features update while the game is paused, so
it may be convenient to test with deathmatch set to 1 so that bringing down the
console doesn't pause the game.

=============================================================================
*/

/*
=================
CG_TestModel_f

Creates an entity in front of the current position, which
can then be moved around
=================
*/
void CG_TestModel_f (void) {
	vec3_t		angles;

	memset( &cg.testModelEntity, 0, sizeof(cg.testModelEntity) );
	if ( trap_Argc() < 2 ) {
		return;
	}

	Q_strncpyz (cg.testModelName, CG_Argv( 1 ), MAX_QPATH );
	cg.testModelEntity.hModel = trap_R_RegisterModel( cg.testModelName );

	if ( trap_Argc() == 3 ) {
		cg.testModelEntity.backlerp = atof( CG_Argv( 2 ) );
		cg.testModelEntity.frame = 1;
		cg.testModelEntity.oldframe = 0;
	}
	if (! cg.testModelEntity.hModel ) {
		CG_Printf( "Can't register model\n" );
		return;
	}

	VectorMA( cg.refdef.vieworg, 100, cg.refdef.viewaxis[0], cg.testModelEntity.origin );

	angles[PITCH] = 0;
	angles[YAW] = 180 + cg.refdef.viewangles[1];
	angles[ROLL] = 0;

	AnglesToAxis( angles, cg.testModelEntity.axis );
	cg.testGun = qfalse;
}

/*
=================
CG_TestGun_f

Replaces the current view weapon with the given model
=================
*/
void CG_TestGun_f (void) {
	CG_TestModel_f();
	cg.testGun = qtrue;
	//cg.testModelEntity.renderfx = RF_MINLIGHT | RF_DEPTHHACK | RF_FIRST_PERSON;

	// rww - 9-13-01 [1-26-01-sof2]
	cg.testModelEntity.renderfx = RF_DEPTHHACK | RF_FIRST_PERSON;
}


void CG_TestModelNextFrame_f (void) {
	cg.testModelEntity.frame++;
	CG_Printf( "frame %i\n", cg.testModelEntity.frame );
}

void CG_TestModelPrevFrame_f (void) {
	cg.testModelEntity.frame--;
	if ( cg.testModelEntity.frame < 0 ) {
		cg.testModelEntity.frame = 0;
	}
	CG_Printf( "frame %i\n", cg.testModelEntity.frame );
}

void CG_TestModelNextSkin_f (void) {
	cg.testModelEntity.skinNum++;
	CG_Printf( "skin %i\n", cg.testModelEntity.skinNum );
}

void CG_TestModelPrevSkin_f (void) {
	cg.testModelEntity.skinNum--;
	if ( cg.testModelEntity.skinNum < 0 ) {
		cg.testModelEntity.skinNum = 0;
	}
	CG_Printf( "skin %i\n", cg.testModelEntity.skinNum );
}

static void CG_AddTestModel (void) {
	int		i;

	// re-register the model, because the level may have changed
	cg.testModelEntity.hModel = trap_R_RegisterModel( cg.testModelName );
	if (! cg.testModelEntity.hModel ) {
		CG_Printf ("Can't register model\n");
		return;
	}

	// if testing a gun, set the origin reletive to the view origin
	if ( cg.testGun ) {
		VectorCopy( cg.refdef.vieworg, cg.testModelEntity.origin );
		VectorCopy( cg.refdef.viewaxis[0], cg.testModelEntity.axis[0] );
		VectorCopy( cg.refdef.viewaxis[1], cg.testModelEntity.axis[1] );
		VectorCopy( cg.refdef.viewaxis[2], cg.testModelEntity.axis[2] );

		// allow the position to be adjusted
		for (i=0 ; i<3 ; i++) {
			cg.testModelEntity.origin[i] += cg.refdef.viewaxis[0][i] * cg_gun_x.value;
			cg.testModelEntity.origin[i] += cg.refdef.viewaxis[1][i] * cg_gun_y.value;
			cg.testModelEntity.origin[i] += cg.refdef.viewaxis[2][i] * cg_gun_z.value;
		}
	}

	trap_R_AddRefEntityToScene( &cg.testModelEntity );
}



//============================================================================


/*
=================
CG_CalcVrect

Sets the coordinates of the rendered window
=================
*/
static void CG_CalcVrect (void) {
	int		size;

	// the intermission should allways be full screen
	if ( cg.snap->ps.pm_type == PM_INTERMISSION ) {
		size = 100;
	} else {
		// bound normal viewsize
		if (cg_viewsize.integer < 30) {
			trap_Cvar_Set ("cg_viewsize","30");
			size = 30;
		} else if (cg_viewsize.integer > 100) {
			trap_Cvar_Set ("cg_viewsize","100");
			size = 100;
		} else {
			size = cg_viewsize.integer;
		}

	}
	cg.refdef.width = cgs.glconfig.vidWidth*size/100;
	cg.refdef.width &= ~1;

	cg.refdef.height = cgs.glconfig.vidHeight*size/100;
	cg.refdef.height &= ~1;

	cg.refdef.x = (cgs.glconfig.vidWidth - cg.refdef.width)/2;
	cg.refdef.y = (cgs.glconfig.vidHeight - cg.refdef.height)/2;
}

//==============================================================================

//==============================================================================
//==============================================================================
// this causes a compiler bug on mac MrC compiler
static void CG_StepOffset( void ) {
	int		timeDelta;
	
	// smooth out stair climbing
	timeDelta = cg.time - cg.stepTime;
	if ( timeDelta < STEP_TIME ) {
		cg.refdef.vieworg[2] -= cg.stepChange 
			* (STEP_TIME - timeDelta) / STEP_TIME;
	}
}

#define CAMERA_DAMP_INTERVAL	50

static vec3_t	cameramins = { -CAMERA_SIZE, -CAMERA_SIZE, -CAMERA_SIZE };
static vec3_t	cameramaxs = { CAMERA_SIZE, CAMERA_SIZE, CAMERA_SIZE };
vec3_t	camerafwd, cameraup;

vec3_t	cameraFocusAngles,			cameraFocusLoc;
vec3_t	cameraIdealTarget,			cameraIdealLoc;
vec3_t	cameraCurTarget={0,0,0},	cameraCurLoc={0,0,0};
vec3_t	cameraOldLoc={0,0,0},		cameraNewLoc={0,0,0};
int		cameraLastFrame=0;

float	cameraLastYaw=0;
float	cameraStiffFactor=0.0f;

/*
===============
Notes on the camera viewpoint in and out...

cg.refdef.vieworg
--at the start of the function holds the player actor's origin (center of player model).
--it is set to the final view location of the camera at the end of the camera code.
cg.refdef.viewangles
--at the start holds the client's view angles
--it is set to the final view angle of the camera at the end of the camera code.

===============
*/
  
extern qboolean gCGHasFallVector;
extern vec3_t gCGFallVector;

/*
===============
CG_CalcTargetThirdPersonViewLocation

===============
*/
static void CG_CalcIdealThirdPersonViewTarget(void)
{
	// Initialize IdealTarget
	if (gCGHasFallVector)
	{
		VectorCopy(gCGFallVector, cameraFocusLoc);
	}
	else
	{
		VectorCopy(cg.refdef.vieworg, cameraFocusLoc);
	}

	// Add in the new viewheight
	cameraFocusLoc[2] += cg.snap->ps.viewheight;

	// Add in a vertical offset from the viewpoint, which puts the actual target above the head, regardless of angle.
//	VectorMA(cameraFocusLoc, thirdPersonVertOffset, cameraup, cameraIdealTarget);
	
	// Add in a vertical offset from the viewpoint, which puts the actual target above the head, regardless of angle.
	VectorCopy( cameraFocusLoc, cameraIdealTarget );
	
	{
		float vertOffset = cg_thirdPersonVertOffset.value;

		if (cg.snap && cg.snap->ps.m_iVehicleNum)
		{
			centity_t *veh = &cg_entities[cg.snap->ps.m_iVehicleNum];
			if (veh->m_pVehicle &&
				veh->m_pVehicle->m_pVehicleInfo->cameraOverride)
			{ //override the range with what the vehicle wants it to be
				if ( veh->m_pVehicle->m_pVehicleInfo->cameraPitchDependantVertOffset )
				{
					if ( cg.snap->ps.viewangles[PITCH] > 0 )
					{
						vertOffset = 130+(pug_demospec.integer!=-1?cg_entities[pug_demospec.integer].lerpAngles[PITCH]:cg.predictedPlayerState.viewangles[PITCH])*-10;
						if ( vertOffset < -170 )
						{
							vertOffset = -170;
						}
					}
					else if ( cg.snap->ps.viewangles[PITCH] < 0 )
					{
						vertOffset = 130+(pug_demospec.integer!=-1?cg_entities[pug_demospec.integer].lerpAngles[PITCH]:cg.predictedPlayerState.viewangles[PITCH])*-5;
						if ( vertOffset > 130 )
						{
							vertOffset = 130;
						}
					}
					else
					{
						vertOffset = 30;
					}
				}
				else 
				{
					vertOffset = veh->m_pVehicle->m_pVehicleInfo->cameraVertOffset;
				}
			}
			else if ( veh->m_pVehicle
				&& veh->m_pVehicle->m_pVehicleInfo
				&& veh->m_pVehicle->m_pVehicleInfo->type == VH_ANIMAL )
			{
				vertOffset = 0;
			}
		}
		cameraIdealTarget[2] += vertOffset;
	}
	//VectorMA(cameraFocusLoc, cg_thirdPersonVertOffset.value, cameraup, cameraIdealTarget);
}

	

/*
===============
CG_CalcTargetThirdPersonViewLocation

===============
*/
static void CG_CalcIdealThirdPersonViewLocation(void)
{
	float thirdPersonRange = cg_thirdPersonRange.value;

	if (cg.snap && cg.snap->ps.m_iVehicleNum)
	{
		centity_t *veh = &cg_entities[cg.snap->ps.m_iVehicleNum];
		if (veh->m_pVehicle &&
			veh->m_pVehicle->m_pVehicleInfo->cameraOverride)
		{ //override the range with what the vehicle wants it to be
			thirdPersonRange = veh->m_pVehicle->m_pVehicleInfo->cameraRange;
			if ( veh->playerState->hackingTime )
			{
				thirdPersonRange += fabs(((float)veh->playerState->hackingTime)/MAX_STRAFE_TIME) * 100.0f;
			}
		}
	}

	if ( cg.snap
		&& (cg.snap->ps.eFlags2&EF2_HELD_BY_MONSTER) 
		&& cg.snap->ps.hasLookTarget
		&& cg_entities[cg.snap->ps.lookTarget].currentState.NPC_class == CLASS_RANCOR )//only possibility for now, may add Wampa and sand creature later
	{//stay back
		//thirdPersonRange = 180.0f;
		thirdPersonRange = 120.0f;
	}

	VectorMA(cameraIdealTarget, -(thirdPersonRange), camerafwd, cameraIdealLoc);
}



static void CG_ResetThirdPersonViewDamp(void)
{
	trace_t trace;

	// Cap the pitch within reasonable limits
	if (cameraFocusAngles[PITCH] > 89.0)
	{
		cameraFocusAngles[PITCH] = 89.0;
	}
	else if (cameraFocusAngles[PITCH] < -89.0)
	{
		cameraFocusAngles[PITCH] = -89.0;
	}

	AngleVectors(cameraFocusAngles, camerafwd, NULL, cameraup);

	// Set the cameraIdealTarget
	CG_CalcIdealThirdPersonViewTarget();

	// Set the cameraIdealLoc
	CG_CalcIdealThirdPersonViewLocation();

	// Now, we just set everything to the new positions.
	VectorCopy(cameraIdealLoc, cameraCurLoc);
	VectorCopy(cameraIdealTarget, cameraCurTarget);

	// First thing we do is trace from the first person viewpoint out to the new target location.
	CG_Trace(&trace, cameraFocusLoc, cameramins, cameramaxs, cameraCurTarget, cg.snap->ps.clientNum, MASK_CAMERACLIP);
	if (trace.fraction <= 1.0)
	{
		VectorCopy(trace.endpos, cameraCurTarget);
	}

	// Now we trace from the new target location to the new view location, to make sure there is nothing in the way.
	CG_Trace(&trace, cameraCurTarget, cameramins, cameramaxs, cameraCurLoc, cg.snap->ps.clientNum, MASK_CAMERACLIP);
	if (trace.fraction <= 1.0)
	{
		VectorCopy(trace.endpos, cameraCurLoc);
	}

	cameraLastFrame = cg.time;
	cameraLastYaw = cameraFocusAngles[YAW];
	cameraStiffFactor = 0.0f;
}

// This is called every frame.
static void CG_UpdateThirdPersonTargetDamp(void)
{
	trace_t trace;
	vec3_t	targetdiff;
	float	dampfactor, dtime, ratio;

	// Set the cameraIdealTarget
	// Automatically get the ideal target, to avoid jittering.
	CG_CalcIdealThirdPersonViewTarget();

	if ( cg.predictedVehicleState.hyperSpaceTime
		&& (cg.time-cg.predictedVehicleState.hyperSpaceTime) < HYPERSPACE_TIME )
	{//hyperspacing, no damp
		VectorCopy(cameraIdealTarget, cameraCurTarget);
	}
	else if (cg_thirdPersonTargetDamp.value>=1.0||cg.thisFrameTeleport||cg.predictedPlayerState.m_iVehicleNum)
	{	// No damping.
		VectorCopy(cameraIdealTarget, cameraCurTarget);
	}
	else if (cg_thirdPersonTargetDamp.value>=0.0)
	{	
		// Calculate the difference from the current position to the new one.
		VectorSubtract(cameraIdealTarget, cameraCurTarget, targetdiff);

		// Now we calculate how much of the difference we cover in the time allotted.
		// The equation is (Damp)^(time)
		dampfactor = 1.0-cg_thirdPersonTargetDamp.value;	// We must exponent the amount LEFT rather than the amount bled off
		dtime = (float)(cg.time-cameraLastFrame) * (1.0/(float)CAMERA_DAMP_INTERVAL);	// Our dampfactor is geared towards a time interval equal to "1".

		// Note that since there are a finite number of "practical" delta millisecond values possible, 
		// the ratio should be initialized into a chart ultimately.
		ratio = FIXEDpowf(dampfactor, dtime);
		
		// This value is how much distance is "left" from the ideal.
		VectorMA(cameraIdealTarget, -ratio, targetdiff, cameraCurTarget);
		/////////////////////////////////////////////////////////////////////////////////////////////////////////
	}

	// Now we trace to see if the new location is cool or not.

	// First thing we do is trace from the first person viewpoint out to the new target location.
	CG_Trace(&trace, cameraFocusLoc, cameramins, cameramaxs, cameraCurTarget, cg.snap->ps.clientNum, MASK_CAMERACLIP);
	if (trace.fraction < 1.0)
	{
		VectorCopy(trace.endpos, cameraCurTarget);
	}

	// Note that previously there was an upper limit to the number of physics traces that are done through the world
	// for the sake of camera collision, since it wasn't calced per frame.  Now it is calculated every frame.
	// This has the benefit that the camera is a lot smoother now (before it lerped between tested points),
	// however two full volume traces each frame is a bit scary to think about.
}

// This can be called every interval, at the user's discretion.
extern void CG_CalcEntityLerpPositions( centity_t *cent ); //cg_ents.c
static void CG_UpdateThirdPersonCameraDamp(void)
{
	trace_t trace;
	vec3_t	locdiff;
	float dampfactor, dtime, ratio;

	// Set the cameraIdealLoc
	CG_CalcIdealThirdPersonViewLocation();
	
	
	// First thing we do is calculate the appropriate damping factor for the camera.
	dampfactor=0.0;
	if ( cg.predictedVehicleState.hyperSpaceTime
		&& (cg.time-cg.predictedVehicleState.hyperSpaceTime) < HYPERSPACE_TIME )
	{//hyperspacing - don't damp camera
		dampfactor = 1.0f;
	}
	else if (cg_thirdPersonCameraDamp.value != 0.0)
	{
		float pitch;
		float dFactor;

		if (!cg.predictedPlayerState.m_iVehicleNum)
		{
			dFactor = cg_thirdPersonCameraDamp.value;
		}
		else
		{
			dFactor = 1.0f;
		}

		// Note that the camera pitch has already been capped off to 89.
		pitch = Q_fabs(cameraFocusAngles[PITCH]);

		// The higher the pitch, the larger the factor, so as you look up, it damps a lot less.
		pitch /= 115.0;	
		dampfactor = (1.0-dFactor)*(pitch*pitch);

		dampfactor += dFactor;

		// Now we also multiply in the stiff factor, so that faster yaw changes are stiffer.
		if (cameraStiffFactor > 0.0f)
		{	// The cameraStiffFactor is how much of the remaining damp below 1 should be shaved off, i.e. approach 1 as stiffening increases.
			dampfactor += (1.0-dampfactor)*cameraStiffFactor;
		}
	}

	if (dampfactor>=1.0||cg.thisFrameTeleport)
	{	// No damping.
		VectorCopy(cameraIdealLoc, cameraCurLoc);
	}
	else if (dampfactor>=0.0)
	{	
		// Calculate the difference from the current position to the new one.
		VectorSubtract(cameraIdealLoc, cameraCurLoc, locdiff);

		// Now we calculate how much of the difference we cover in the time allotted.
		// The equation is (Damp)^(time)
		dampfactor = 1.0-dampfactor;	// We must exponent the amount LEFT rather than the amount bled off
		dtime = (float)(cg.time-cameraLastFrame) * (1.0/(float)CAMERA_DAMP_INTERVAL);	// Our dampfactor is geared towards a time interval equal to "1".

		// Note that since there are a finite number of "practical" delta millisecond values possible, 
		// the ratio should be initialized into a chart ultimately.
		ratio = FIXEDpowf(dampfactor, dtime);
		
		// This value is how much distance is "left" from the ideal.
		VectorMA(cameraIdealLoc, -ratio, locdiff, cameraCurLoc);
		/////////////////////////////////////////////////////////////////////////////////////////////////////////
	}

	// Now we trace from the new target location to the new view location, to make sure there is nothing in the way.
	CG_Trace(&trace, cameraCurTarget, cameramins, cameramaxs, cameraCurLoc, cg.snap->ps.clientNum, MASK_CAMERACLIP);

	if (trace.fraction < 1.0)
	{
		if (trace.entityNum < ENTITYNUM_WORLD &&
			cg_entities[trace.entityNum].currentState.solid == SOLID_BMODEL &&
			cg_entities[trace.entityNum].currentState.eType == ET_MOVER)
		{ //get a different position for movers -rww
			centity_t *mover = &cg_entities[trace.entityNum];

			//this is absolutely hackiful, since we calc view values before we add packet ents and lerp,
			//if we hit a mover we want to update its lerp pos and force it when we do the trace against
			//it.
			if (mover->currentState.pos.trType != TR_STATIONARY &&
				mover->currentState.pos.trType != TR_LINEAR)
			{
				int curTr = mover->currentState.pos.trType;
				vec3_t curTrB;

				VectorCopy(mover->currentState.pos.trBase, curTrB);

				//calc lerporigin for this client frame
				CG_CalcEntityLerpPositions(mover);

				//force the calc'd lerp to be the base and say we are stationary so we don't try to extrapolate
				//out further.
				mover->currentState.pos.trType = TR_STATIONARY;
				VectorCopy(mover->lerpOrigin, mover->currentState.pos.trBase);
				
				//retrace
				CG_Trace(&trace, cameraCurTarget, cameramins, cameramaxs, cameraCurLoc, cg.snap->ps.clientNum, MASK_CAMERACLIP);

				//copy old data back in
				mover->currentState.pos.trType = (trType_t) curTr;
				VectorCopy(curTrB, mover->currentState.pos.trBase);
			}
			if (trace.fraction < 1.0f)
			{ //still hit it, so take the proper trace endpos and use that.
				VectorCopy(trace.endpos, cameraCurLoc);
			}
		}
		else
		{
			VectorCopy( trace.endpos, cameraCurLoc );
		}
	}

	// Note that previously there was an upper limit to the number of physics traces that are done through the world
	// for the sake of camera collision, since it wasn't calced per frame.  Now it is calculated every frame.
	// This has the benefit that the camera is a lot smoother now (before it lerped between tested points),
	// however two full volume traces each frame is a bit scary to think about.
}




/*
===============`
CG_OffsetThirdPersonView

===============
*/
extern vmCvar_t cg_thirdPersonHorzOffset;
extern qboolean BG_UnrestrainedPitchRoll( playerState_t *ps, Vehicle_t *pVeh );
static void CG_OffsetThirdPersonView( void ) 
{
	vec3_t diff;
	float thirdPersonHorzOffset = cg_thirdPersonHorzOffset.value;
	float deltayaw;

	if (cg.snap && cg.snap->ps.m_iVehicleNum)
	{
		centity_t *veh = &cg_entities[cg.snap->ps.m_iVehicleNum];
		if (veh->m_pVehicle &&
			veh->m_pVehicle->m_pVehicleInfo->cameraOverride)
		{ //override the range with what the vehicle wants it to be
			thirdPersonHorzOffset = veh->m_pVehicle->m_pVehicleInfo->cameraHorzOffset;
			if ( veh->playerState->hackingTime )
			{
				thirdPersonHorzOffset += (((float)veh->playerState->hackingTime)/MAX_STRAFE_TIME) * -80.0f;
			}
		}
	}

	cameraStiffFactor = 0.0;

	// Set camera viewing direction.
	VectorCopy( cg.refdef.viewangles, cameraFocusAngles );

	// if dead, look at killer
	if ( cg.snap
		&& (cg.snap->ps.eFlags2&EF2_HELD_BY_MONSTER) 
		&& cg.snap->ps.hasLookTarget
		&& cg_entities[cg.snap->ps.lookTarget].currentState.NPC_class == CLASS_RANCOR )//only possibility for now, may add Wampa and sand creature later
	{//being held
		//vec3_t monsterPos, dir2Me;
		centity_t	*monster = &cg_entities[cg.snap->ps.lookTarget];
		VectorSet( cameraFocusAngles, 0, AngleNormalize180(monster->lerpAngles[YAW]+180), 0 );
		//make the look angle the vector from his mouth to me
		/*
		VectorCopy( monster->lerpOrigin, monsterPos );
		monsterPos[2] = cg.snap->ps.origin[2];
		VectorSubtract( monsterPos, cg.snap->ps.origin, dir2Me );
		vectoangles( dir2Me, cameraFocusAngles );
		*/
	}
	else if ( cg.snap->ps.stats[STAT_HEALTH] <= 0 ) 
	{
		cameraFocusAngles[YAW] = cg.snap->ps.stats[STAT_DEAD_YAW];
	}
	else
	{	// Add in the third Person Angle.
		cameraFocusAngles[YAW] += cg_thirdPersonAngle.value;
		{
			float pitchOffset = cg_thirdPersonPitchOffset.value;
			if (cg.snap && cg.snap->ps.m_iVehicleNum)
			{
				centity_t *veh = &cg_entities[cg.snap->ps.m_iVehicleNum];
				if (veh->m_pVehicle &&
					veh->m_pVehicle->m_pVehicleInfo->cameraOverride)
				{ //override the range with what the vehicle wants it to be
					if ( veh->m_pVehicle->m_pVehicleInfo->cameraPitchDependantVertOffset )
					{
						if ( (pug_demospec.integer!=-1?cg_entities[pug_demospec.integer].lerpAngles[PITCH]:cg.snap->ps.viewangles[PITCH]) > 0 )
						{
							pitchOffset = (pug_demospec.integer!=-1?cg_entities[pug_demospec.integer].lerpAngles[PITCH]:cg.predictedPlayerState.viewangles[PITCH])*-0.75;
						}
						else if ( (pug_demospec.integer!=-1?cg_entities[pug_demospec.integer].lerpAngles[PITCH]:cg.snap->ps.viewangles[PITCH]) < 0 )
						{
							pitchOffset = (pug_demospec.integer!=-1?cg_entities[pug_demospec.integer].lerpAngles[PITCH]:cg.predictedPlayerState.viewangles[PITCH])*-0.75;
						}
						else
						{
							pitchOffset = 0;
						}
					}
					else
					{
						pitchOffset = veh->m_pVehicle->m_pVehicleInfo->cameraPitchOffset;
					}
				}
			}
			if ( 0 && cg.predictedPlayerState.m_iVehicleNum //in a vehicle
				&& BG_UnrestrainedPitchRoll( &cg.predictedPlayerState, cg_entities[cg.predictedPlayerState.m_iVehicleNum].m_pVehicle ) )//can roll/pitch without restriction
			{
				float pitchPerc = ((90.0f-fabs(cameraFocusAngles[ROLL]))/90.0f);
				cameraFocusAngles[PITCH] += pitchOffset*pitchPerc;
				if ( cameraFocusAngles[ROLL] > 0 )
				{
					cameraFocusAngles[YAW] -= pitchOffset-(pitchOffset*pitchPerc);
				}
				else
				{
					cameraFocusAngles[YAW] += pitchOffset-(pitchOffset*pitchPerc);
				}
			}
			else
			{
				cameraFocusAngles[PITCH] += pitchOffset;
			}
		}
	}

	// The next thing to do is to see if we need to calculate a new camera target location.

	// If we went back in time for some reason, or if we just started, reset the sample.
	if (cameraLastFrame == 0 || cameraLastFrame > cg.time )
	{
		CG_ResetThirdPersonViewDamp();
	}
	else
	{
		// Cap the pitch within reasonable limits
		if ( cg.predictedPlayerState.m_iVehicleNum //in a vehicle
			&& BG_UnrestrainedPitchRoll( &cg.predictedPlayerState, cg_entities[cg.predictedPlayerState.m_iVehicleNum].m_pVehicle ) )//can roll/pitch without restriction
		{//no clamp on pitch
			//FIXME: when pitch >= 90 or <= -90, camera rotates oddly... need to CrossProduct not just vectoangles
		}
		else
		{
			if (cameraFocusAngles[PITCH] > 80.0)
			{
				cameraFocusAngles[PITCH] = 80.0;
			}
			else if (cameraFocusAngles[PITCH] < -80.0)
			{
				cameraFocusAngles[PITCH] = -80.0;
			}
		}

		AngleVectors(cameraFocusAngles, camerafwd, NULL, cameraup);

		deltayaw = fabs(cameraFocusAngles[YAW] - cameraLastYaw);
		if (deltayaw > 180.0f)
		{ // Normalize this angle so that it is between 0 and 180.
			deltayaw = fabs(deltayaw - 360.0f);
		}
		cameraStiffFactor = deltayaw / (float)(cg.time-cameraLastFrame);
		if (cameraStiffFactor < 1.0)
		{
			cameraStiffFactor = 0.0;
		}
		else if (cameraStiffFactor > 2.5)
		{
			cameraStiffFactor = 0.75;
		}
		else
		{	// 1 to 2 scales from 0.0 to 0.5
			cameraStiffFactor = (cameraStiffFactor-1.0f)*0.5f;
		}
		cameraLastYaw = cameraFocusAngles[YAW];

		// Move the target to the new location.
		CG_UpdateThirdPersonTargetDamp();
		CG_UpdateThirdPersonCameraDamp();
	}

	// Now interestingly, the Quake method is to calculate a target focus point above the player, and point the camera at it.
	// We won't do that for now.

	// We must now take the angle taken from the camera target and location.
	/*VectorSubtract(cameraCurTarget, cameraCurLoc, diff);
	VectorNormalize(diff);
	vectoangles(diff, cg.refdef.viewangles);*/
	VectorSubtract(cameraCurTarget, cameraCurLoc, diff);
	{
		float dist = VectorNormalize(diff);
		//under normal circumstances, should never be 0.00000 and so on.
		if ( !dist || (diff[0] == 0 || diff[1] == 0) )
		{//must be hitting something, need some value to calc angles, so use cam forward
			VectorCopy( camerafwd, diff );
		}
	}
	if ( 0 && cg.predictedPlayerState.m_iVehicleNum //in a vehicle
		&& BG_UnrestrainedPitchRoll( &cg.predictedPlayerState, cg_entities[cg.predictedPlayerState.m_iVehicleNum].m_pVehicle ) )//can roll/pitch without restriction
	{//FIXME: this causes camera jerkiness, need to blend the roll?
		float sav_Roll = cg.refdef.viewangles[ROLL];
		vectoangles(diff, cg.refdef.viewangles);
		cg.refdef.viewangles[ROLL] = sav_Roll;
	}
	else
	{
		vectoangles(diff, cg.refdef.viewangles);
	}

	// Temp: just move the camera to the side a bit
	if ( thirdPersonHorzOffset != 0.0f )
	{
		AnglesToAxis( cg.refdef.viewangles, cg.refdef.viewaxis );
		VectorMA( cameraCurLoc, thirdPersonHorzOffset, cg.refdef.viewaxis[1], cameraCurLoc );
	}

	// ...and of course we should copy the new view location to the proper spot too.
	VectorCopy(cameraCurLoc, cg.refdef.vieworg);

	cameraLastFrame=cg.time;
}

void CG_GetVehicleCamPos( vec3_t camPos )
{
	VectorCopy( cg.refdef.vieworg, camPos );
}

/*
===============
CG_OffsetThirdPersonView

===============
*//*
#define	FOCUS_DISTANCE	512
static void CG_OffsetThirdPersonView( void ) {
	vec3_t		forward, right, up;
	vec3_t		view;
	vec3_t		focusAngles;
	trace_t		trace;
	static vec3_t	mins = { -4, -4, -4 };
	static vec3_t	maxs = { 4, 4, 4 };
	vec3_t		focusPoint;
	float		focusDist;
	float		forwardScale, sideScale;

	cg.refdef.vieworg[2] += cg.predictedPlayerState.viewheight;

	VectorCopy( cg.refdef.viewangles, focusAngles );

	// if dead, look at killer
	if ( cg.predictedPlayerState.stats[STAT_HEALTH] <= 0 ) {
		focusAngles[YAW] = cg.predictedPlayerState.stats[STAT_DEAD_YAW];
		cg.refdef.viewangles[YAW] = cg.predictedPlayerState.stats[STAT_DEAD_YAW];
	}

	if ( focusAngles[PITCH] > 45 ) {
		focusAngles[PITCH] = 45;		// don't go too far overhead
	}
	AngleVectors( focusAngles, forward, NULL, NULL );

	VectorMA( cg.refdef.vieworg, FOCUS_DISTANCE, forward, focusPoint );

	VectorCopy( cg.refdef.vieworg, view );

	view[2] += 8;

	cg.refdef.viewangles[PITCH] *= 0.5;

	AngleVectors( cg.refdef.viewangles, forward, right, up );

	forwardScale = cos( cg_thirdPersonAngle.value / 180 * M_PI );
	sideScale = sin( cg_thirdPersonAngle.value / 180 * M_PI );
	VectorMA( view, -cg_thirdPersonRange.value * forwardScale, forward, view );
	VectorMA( view, -cg_thirdPersonRange.value * sideScale, right, view );

	// trace a ray from the origin to the viewpoint to make sure the view isn't
	// in a solid block.  Use an 8 by 8 block to prevent the view from near clipping anything

	if (!cg_cameraMode.integer) {
		CG_Trace( &trace, cg.refdef.vieworg, mins, maxs, view, cg.predictedPlayerState.clientNum, MASK_CAMERACLIP);

		if ( trace.fraction != 1.0 ) {
			VectorCopy( trace.endpos, view );
			view[2] += (1.0 - trace.fraction) * 32;
			// try another trace to this position, because a tunnel may have the ceiling
			// close enogh that this is poking out

			CG_Trace( &trace, cg.refdef.vieworg, mins, maxs, view, cg.predictedPlayerState.clientNum, MASK_CAMERACLIP);
			VectorCopy( trace.endpos, view );
		}
	}


	VectorCopy( view, cg.refdef.vieworg );

	// select pitch to look at focus point from vieword
	VectorSubtract( focusPoint, cg.refdef.vieworg, focusPoint );
	focusDist = sqrt( focusPoint[0] * focusPoint[0] + focusPoint[1] * focusPoint[1] );
	if ( focusDist < 1 ) {
		focusDist = 1;	// should never happen
	}
	cg.refdef.viewangles[PITCH] = -180 / M_PI * atan2( focusPoint[2], focusDist );
	cg.refdef.viewangles[YAW] -= cg_thirdPersonAngle.value;
}


// this causes a compiler bug on mac MrC compiler
static void CG_StepOffset( void ) {
	int		timeDelta;
	
	// smooth out stair climbing
	timeDelta = cg.time - cg.stepTime;
	if ( timeDelta < STEP_TIME ) {
		cg.refdef.vieworg[2] -= cg.stepChange 
			* (STEP_TIME - timeDelta) / STEP_TIME;
	}
}*/

/*
===============
CG_OffsetFirstPersonView

===============
*/
static void CG_OffsetFirstPersonView( void ) {
	float			*origin;
	float			*angles;
	float			bob;
	float			ratio;
	float			delta;
	float			speed;
	float			f;
	vec3_t			predictedVelocity;
	int				timeDelta;
	int				kickTime;
	
	if ( cg.snap->ps.pm_type == PM_INTERMISSION ) {
		return;
	}

	origin = cg.refdef.vieworg;
	angles = cg.refdef.viewangles;

	// if dead, fix the angle and don't add any kick
	if ( cg.snap->ps.stats[STAT_HEALTH] <= 0 ) {
		angles[ROLL] = 40;
		angles[PITCH] = -15;
		angles[YAW] = cg.snap->ps.stats[STAT_DEAD_YAW];
		origin[2] += cg.predictedPlayerState.viewheight;
		return;
	}

	// add angles based on weapon kick
	kickTime = (cg.time - cg.kick_time);
	if ( kickTime < 800 )
	{//kicks are always 1 second long.  Deal with it.
		float kickPerc = 0.0f;
		if ( kickTime <= 200 )
		{//winding up
			kickPerc = kickTime/200.0f;
		}
		else
		{//returning to normal
			kickTime = 800 - kickTime;
			kickPerc = kickTime/600.0f;
		}
		VectorMA( angles, kickPerc, cg.kick_angles, angles );
	}
	// add angles based on damage kick
	if ( cg.damageTime ) {
		ratio = cg.time - cg.damageTime;
		if ( ratio < DAMAGE_DEFLECT_TIME ) {
			ratio /= DAMAGE_DEFLECT_TIME;
			angles[PITCH] += ratio * cg.v_dmg_pitch;
			angles[ROLL] += ratio * cg.v_dmg_roll;
		} else {
			ratio = 1.0 - ( ratio - DAMAGE_DEFLECT_TIME ) / DAMAGE_RETURN_TIME;
			if ( ratio > 0 ) {
				angles[PITCH] += ratio * cg.v_dmg_pitch;
				angles[ROLL] += ratio * cg.v_dmg_roll;
			}
		}
	}

	// add pitch based on fall kick
#if 0
	ratio = ( cg.time - cg.landTime) / FALL_TIME;
	if (ratio < 0)
		ratio = 0;
	angles[PITCH] += ratio * cg.fall_value;
#endif

	// add angles based on velocity
	if(pug_demospec.integer==-1){VectorCopy( cg.predictedPlayerState.velocity, predictedVelocity );

	delta = DotProduct ( predictedVelocity, cg.refdef.viewaxis[0]);
	angles[PITCH] += delta * cg_runpitch.value;
	
	delta = DotProduct ( predictedVelocity, cg.refdef.viewaxis[1]);
	angles[ROLL] -= delta * cg_runroll.value;}

	// add angles based on bob

	// make sure the bob is visible even at low speeds
	speed = cg.xyspeed > 200 ? cg.xyspeed : 200;

	delta = cg.bobfracsin * cg_bobpitch.value * speed;
	if (cg.predictedPlayerState.pm_flags & PMF_DUCKED)
		delta *= 3;		// crouching
	angles[PITCH] += delta;
	delta = cg.bobfracsin * cg_bobroll.value * speed;
	if (cg.predictedPlayerState.pm_flags & PMF_DUCKED)
		delta *= 3;		// crouching accentuates roll
	if (cg.bobcycle & 1)
		delta = -delta;
	angles[ROLL] += delta;

//===================================

	// add view height
	origin[2] += cg.predictedPlayerState.viewheight;
	
	// smooth out duck height changes
	timeDelta = cg.time - cg.duckTime;
	if ( timeDelta < DUCK_TIME && pug_demospec.integer==-1) {
		cg.refdef.vieworg[2] -= cg.duckChange 
			* (DUCK_TIME - timeDelta) / DUCK_TIME;
	}

	// add bob height
	bob = cg.bobfracsin * cg.xyspeed * cg_bobup.value;
	if (bob > 6) {
		bob = 6;
	}

	origin[2] += bob;


	// add fall height
	delta = cg.time - cg.landTime;
	if ( delta < LAND_DEFLECT_TIME ) {
		f = delta / LAND_DEFLECT_TIME;
		cg.refdef.vieworg[2] += cg.landChange * f;
	} else if ( delta < LAND_DEFLECT_TIME + LAND_RETURN_TIME ) {
		delta -= LAND_DEFLECT_TIME;
		f = 1.0 - ( delta / LAND_RETURN_TIME );
		cg.refdef.vieworg[2] += cg.landChange * f;
	}
	
	// add step offset
	CG_StepOffset();
	
	// add kick offset

	VectorAdd (origin, cg.kick_origin, origin);
	

	// pivot the eye based on a neck length
#if 0
	{
#define	NECK_LENGTH		8
	vec3_t			forward, up;
 
	cg.refdef.vieworg[2] -= NECK_LENGTH;
	AngleVectors( cg.refdef.viewangles, forward, NULL, up );
	VectorMA( cg.refdef.vieworg, 3, forward, cg.refdef.vieworg );
	VectorMA( cg.refdef.vieworg, NECK_LENGTH, up, cg.refdef.vieworg );
	}
#endif
}

static void CG_OffsetFighterView( void )
{
	vec3_t vehFwd, vehRight, vehUp, backDir;
	vec3_t	camOrg, camBackOrg;
	float horzOffset = cg_thirdPersonHorzOffset.value;
	float vertOffset = cg_thirdPersonVertOffset.value;
	float pitchOffset = cg_thirdPersonPitchOffset.value;
	float yawOffset = cg_thirdPersonAngle.value;
	float range = cg_thirdPersonRange.value;
	trace_t	trace;
	centity_t *veh = &cg_entities[cg.predictedPlayerState.m_iVehicleNum];

	AngleVectors( cg.refdef.viewangles, vehFwd, vehRight, vehUp );

	if ( veh->m_pVehicle &&
		veh->m_pVehicle->m_pVehicleInfo->cameraOverride )
	{ //override the horizontal offset with what the vehicle wants it to be
		horzOffset = veh->m_pVehicle->m_pVehicleInfo->cameraHorzOffset;
		vertOffset = veh->m_pVehicle->m_pVehicleInfo->cameraVertOffset;
		//NOTE: no yaw offset?
		pitchOffset = veh->m_pVehicle->m_pVehicleInfo->cameraPitchOffset;
		range = veh->m_pVehicle->m_pVehicleInfo->cameraRange;
		if ( veh->playerState->hackingTime )
		{
			horzOffset += (((float)veh->playerState->hackingTime)/MAX_STRAFE_TIME) * -80.0f;
			range += fabs(((float)veh->playerState->hackingTime)/MAX_STRAFE_TIME) * 100.0f;
		}
	}

	//Set camera viewing position
	VectorMA( cg.refdef.vieworg, horzOffset, vehRight, camOrg );
	VectorMA( camOrg, vertOffset, vehUp, camOrg );

	//trace to that pos
	CG_Trace(&trace, cg.refdef.vieworg, cameramins, cameramaxs, camOrg, cg.snap->ps.clientNum, MASK_CAMERACLIP);
	if ( trace.fraction < 1.0 )
	{
		VectorCopy( trace.endpos, camOrg );
	}

	// Set camera viewing direction.
	cg.refdef.viewangles[YAW] += yawOffset;
	cg.refdef.viewangles[PITCH] += pitchOffset;

	//Now bring the cam back from that pos and angles at range
	AngleVectors( cg.refdef.viewangles, backDir, NULL, NULL );
	VectorScale( backDir, -1, backDir );

	VectorMA( camOrg, range, backDir, camBackOrg );

	//trace to that pos
	CG_Trace(&trace, camOrg, cameramins, cameramaxs, camBackOrg, cg.snap->ps.clientNum, MASK_CAMERACLIP);
	VectorCopy( trace.endpos, camOrg );

	//FIXME: do we need to smooth the org?
	// ...and of course we should copy the new view location to the proper spot too.
	VectorCopy(camOrg, cg.refdef.vieworg);
}
//======================================================================

void CG_ZoomDown_f( void ) { 
	if ( cg.zoomed ) {
		return;
	}
	cg.zoomed = qtrue;
	cg.zoomTime = cg.time;
}

void CG_ZoomUp_f( void ) { 
	if ( !cg.zoomed ) {
		return;
	}
	cg.zoomed = qfalse;
	cg.zoomTime = cg.time;
}



/*
====================
CG_CalcFovFromX

Calcs Y FOV from given X FOV
====================
*/
qboolean CG_CalcFOVFromX( float fov_x ) 
{
	float	x;
//	float	phase;
//	float	v;
//	int		contents;
	float	fov_y;
	qboolean	inwater;

	x = cg.refdef.width / tan( fov_x / 360 * M_PI );
	fov_y = atan2( cg.refdef.height, x );
	fov_y = fov_y * 360 / M_PI;

	// there's a problem with this, it only takes the leafbrushes into account, not the entity brushes,
	//	so if you give slime/water etc properties to a func_door area brush in order to move the whole water 
	//	level up/down this doesn't take into account the door position, so warps the view the whole time
	//	whether the water is up or not. Fortunately there's only one slime area in Trek that you can be under,
	//	so lose it...
#if 0
/*
	// warp if underwater
	contents = CG_PointContents( cg.refdef.vieworg, -1 );
	if ( contents & ( CONTENTS_WATER | CONTENTS_SLIME | CONTENTS_LAVA ) ){
		phase = cg.time / 1000.0 * WAVE_FREQUENCY * M_PI * 2;
		v = WAVE_AMPLITUDE * sin( phase );
		fov_x += v;
		fov_y -= v;
		inwater = qtrue;
	}
	else {
		inwater = qfalse;
	}
*/
#else
	inwater = qfalse;
#endif


	// set it
	cg.refdef.fov_x = fov_x;
	cg.refdef.fov_y = fov_y;

#ifdef _XBOX
	if(cg.widescreen)
		cg.refdef.fov_x *= 1.125f;
#endif

	return (inwater);
}

/*
====================
CG_CalcFov

Fixed fov at intermissions, otherwise account for fov variable and zooms.
====================
*/
#define	WAVE_AMPLITUDE	1
#define	WAVE_FREQUENCY	0.4
float zoomFov; //this has to be global client-side

static int CG_CalcFov( void ) {
	float	x;
	float	phase;
	float	v;
	float	fov_x, fov_y;
	float	f;
	int		inwater;
	float	cgFov = cg_fov.value;

	if (cgFov < 1)
	{
		cgFov = 1;
	}
	if (cgFov > 97 && !cg.demoPlayback)
	{
		cgFov = 97;
	}

	if ( cg.predictedPlayerState.pm_type == PM_INTERMISSION ) {
		// if in intermission, use a fixed value
		fov_x = 80;//90;
	} else {
		// user selectable
		if ( cgs.dmflags & DF_FIXED_FOV ) {
			// dmflag to prevent wide fov for all clients
			fov_x = 80;//90;
		} else {
			fov_x = cgFov;
			if ( fov_x < 1 ) {
				fov_x = 1;
			} else if ( fov_x > 160 ) {
				fov_x = 160;
			}
		}

		if (cg.predictedPlayerState.zoomMode == 2 && pug_demos_cameraMode.integer < 3 && !ui_noScope.integer)
		{ //binoculars
			if (zoomFov > 40.0f)
			{
				zoomFov -= cg.frametime * 0.075f;

				if (zoomFov < 40.0f)
				{
					zoomFov = 40.0f;
				}
				else if (zoomFov > cgFov)
				{
					zoomFov = cgFov;
				}
			}

			fov_x = zoomFov;
		}
		else if (cg.predictedPlayerState.zoomMode && pug_demos_cameraMode.integer < 3 && !ui_noScope.integer)
		{
			if (!cg.predictedPlayerState.zoomLocked)
			{
				if (zoomFov > 50)
				{ //Now starting out at nearly half zoomed in
					zoomFov = 50;
				}
				zoomFov -= cg.frametime * 0.035f;//0.075f;

				if (zoomFov < MAX_ZOOM_FOV)
				{
					zoomFov = MAX_ZOOM_FOV;
				}
				else if (zoomFov > cgFov)
				{
					zoomFov = cgFov;
				}
				else
				{	// Still zooming
					static int zoomSoundTime = 0;

					if (zoomSoundTime < cg.time || zoomSoundTime > cg.time + 10000)
					{
						trap_S_StartSound(cg.refdef.vieworg, ENTITYNUM_WORLD, CHAN_LOCAL, cgs.media.disruptorZoomLoop);
						zoomSoundTime = cg.time + 300;
					}
				}
			}

			if (zoomFov < MAX_ZOOM_FOV)
			{
				zoomFov = 50;		// hack to fix zoom during vid restart
			}
			fov_x = zoomFov;
		}
		else 
		{
			zoomFov = 80;

			f = ( cg.time - cg.predictedPlayerState.zoomTime ) / ZOOM_OUT_TIME;
			if ( f > 1.0 || pug_demos_cameraMode.integer > 2 || ui_noScope.integer )
			{
				fov_x = fov_x;
			} 
			else 
			{
				fov_x = cg.predictedPlayerState.zoomFov + f * ( fov_x - cg.predictedPlayerState.zoomFov );
			}
		}
	}

	x = cg.refdef.width / tan( fov_x / 360 * M_PI );
	fov_y = atan2( cg.refdef.height, x );
	fov_y = fov_y * 360 / M_PI;

	// warp if underwater
	cg.refdef.viewContents = CG_PointContents( cg.refdef.vieworg, -1 );
	if ( cg.refdef.viewContents & ( CONTENTS_WATER | CONTENTS_SLIME | CONTENTS_LAVA ) ){
		phase = cg.time / 1000.0 * WAVE_FREQUENCY * M_PI * 2;
		v = WAVE_AMPLITUDE * sin( phase );
		fov_x += v;
		fov_y -= v;
		inwater = qtrue;
	}
	else {
		inwater = qfalse;
	}

#ifdef _XBOX
	if(cg.widescreen)
		fov_x = fov_y * 1.77777f;
#endif


	// set it
	cg.refdef.fov_x = fov_x;
	cg.refdef.fov_y = fov_y;

	if (cg.predictedPlayerState.zoomMode)
	{
		cg.zoomSensitivity = zoomFov/cgFov;
	}
	else if ( !cg.zoomed ) {
		cg.zoomSensitivity = 1;
	} else {
		cg.zoomSensitivity = cg.refdef.fov_y / 75.0;
	}

	return inwater;
}


/*
===============
CG_DamageBlendBlob

===============
*/
static void CG_DamageBlendBlob( void ) 
{
	int			t;
	int			maxTime;
	refEntity_t		ent;

	if ( !cg.damageValue ) {
		return;
	}

	maxTime = DAMAGE_TIME;
	t = cg.time - cg.damageTime;
	if ( t <= 0 || t >= maxTime ) {
		return;
	}

	memset( &ent, 0, sizeof( ent ) );
	ent.reType = RT_SPRITE;
	ent.renderfx = RF_FIRST_PERSON;

	VectorMA( cg.refdef.vieworg, 8, cg.refdef.viewaxis[0], ent.origin );
	VectorMA( ent.origin, cg.damageX * -8, cg.refdef.viewaxis[1], ent.origin );
	VectorMA( ent.origin, cg.damageY * 8, cg.refdef.viewaxis[2], ent.origin );

	ent.radius = cg.damageValue * 3 * ( 1.0 - ((float)t / maxTime) );

	if (cg.snap->ps.damageType == 0)
	{ //pure health
		ent.customShader = cgs.media.viewPainShader;
		ent.shaderRGBA[0] = 180 * ( 1.0 - ((float)t / maxTime) );
		ent.shaderRGBA[1] = 50 * ( 1.0 - ((float)t / maxTime) );
		ent.shaderRGBA[2] = 50 * ( 1.0 - ((float)t / maxTime) );
		ent.shaderRGBA[3] = 255;
	}
	else if (cg.snap->ps.damageType == 1)
	{ //pure shields
		ent.customShader = cgs.media.viewPainShader_Shields;
		ent.shaderRGBA[0] = 50 * ( 1.0 - ((float)t / maxTime) );
		ent.shaderRGBA[1] = 180 * ( 1.0 - ((float)t / maxTime) );
		ent.shaderRGBA[2] = 50 * ( 1.0 - ((float)t / maxTime) );
		ent.shaderRGBA[3] = 255;
	}
	else
	{ //shields and health
		ent.customShader = cgs.media.viewPainShader_ShieldsAndHealth;
		ent.shaderRGBA[0] = 180 * ( 1.0 - ((float)t / maxTime) );
		ent.shaderRGBA[1] = 180 * ( 1.0 - ((float)t / maxTime) );
		ent.shaderRGBA[2] = 50 * ( 1.0 - ((float)t / maxTime) );
		ent.shaderRGBA[3] = 255;
	}
	trap_R_AddRefEntityToScene( &ent );
}

int cg_actionCamLastTime = 0;
vec3_t cg_actionCamLastPos;

//action cam routine -rww
static qboolean CG_ThirdPersonActionCam(void)
{
    centity_t *cent = &cg_entities[cg.snap->ps.clientNum];
	clientInfo_t *ci = &cgs.clientinfo[cg.snap->ps.clientNum];
	trace_t tr;
	vec3_t positionDir;
	vec3_t desiredAngles;
	vec3_t desiredPos;
	vec3_t v;
	const float smoothFactor = 0.1f*cg_timescale.value;
	int i;

	if (!cent->ghoul2)
	{ //if we don't have a g2 instance this frame for whatever reason then do nothing
		return qfalse;
	}

	if (cent->currentState.weapon != WP_SABER)
	{ //just being safe, should not ever happen
		return qfalse;
	}

	if ((cg.time - ci->saber[0].blade[0].trail.lastTime) > 300)
	{ //too long since we last got the blade position
		return qfalse;
	}

	//get direction from base to ent origin
	VectorSubtract(ci->saber[0].blade[0].trail.base, cent->lerpOrigin, positionDir);
	VectorNormalize(positionDir);

	//position the cam based on the direction and saber position
	VectorMA(cent->lerpOrigin, cg_thirdPersonRange.value*2, positionDir, desiredPos);

	//trace to the desired pos to see how far that way we can actually go before we hit something
	//the endpos will be valid for our desiredpos no matter what
	CG_Trace(&tr, cent->lerpOrigin, NULL, NULL, desiredPos, cent->currentState.number, MASK_SOLID);
	VectorCopy(tr.endpos, desiredPos);

	if ((cg.time - cg_actionCamLastTime) > 300)
	{
		//do a third person offset first and grab the initial point from that
		CG_OffsetThirdPersonView();
		VectorCopy(cg.refdef.vieworg, cg_actionCamLastPos);
	}

	cg_actionCamLastTime = cg.time;

	//lerp the vieworg to the desired pos from the last valid
	VectorSubtract(desiredPos, cg_actionCamLastPos, v);
	
	if (VectorLength(v) > 64.0f)
	{ //don't bother moving yet if not far from the last pos
		for (i = 0; i < 3; i++)
		{
			cg_actionCamLastPos[i] = (cg_actionCamLastPos[i] + (v[i]*smoothFactor));
			cg.refdef.vieworg[i] = cg_actionCamLastPos[i];
		}
	}
	else
	{
		VectorCopy(cg_actionCamLastPos, cg.refdef.vieworg);
	}

	//Make sure the point is alright
	CG_Trace(&tr, cent->lerpOrigin, NULL, NULL, cg.refdef.vieworg, cent->currentState.number, MASK_SOLID);
	VectorCopy(tr.endpos, cg.refdef.vieworg);

	VectorSubtract(cent->lerpOrigin, cg.refdef.vieworg, positionDir);
	vectoangles(positionDir, desiredAngles);

	//just set the angles for now
	VectorCopy(desiredAngles, cg.refdef.viewangles);
	return qtrue;
}

vec3_t	cg_lastTurretViewAngles={0};
qboolean CG_CheckPassengerTurretView( void )
{
	if ( cg.predictedPlayerState.m_iVehicleNum //in a vehicle
		&& cg.predictedPlayerState.generic1 )//as a passenger
	{//passenger in a vehicle
		centity_t *vehCent = &cg_entities[cg.predictedPlayerState.m_iVehicleNum];
		if ( vehCent->m_pVehicle
			&& vehCent->m_pVehicle->m_pVehicleInfo 
			&& vehCent->m_pVehicle->m_pVehicleInfo->maxPassengers )
		{//a vehicle capable of carrying passengers
			int turretNum;
			for ( turretNum = 0; turretNum < MAX_VEHICLE_TURRETS; turretNum++ )
			{
				if ( vehCent->m_pVehicle->m_pVehicleInfo->turret[turretNum].iAmmoMax )
				{// valid turret
					if ( vehCent->m_pVehicle->m_pVehicleInfo->turret[turretNum].passengerNum == cg.predictedPlayerState.generic1 )
					{//I control this turret
						int boltIndex = -1;
						qboolean hackPosAndAngle = qfalse;
						if ( vehCent->m_pVehicle->m_iGunnerViewTag[turretNum] != -1 )
						{
							boltIndex = vehCent->m_pVehicle->m_iGunnerViewTag[turretNum];
						}
						else
						{//crap... guess?
							hackPosAndAngle = qtrue;
							if ( vehCent->m_pVehicle->m_pVehicleInfo->turret[turretNum].yawBone )
							{
								boltIndex = trap_G2API_AddBolt( vehCent->ghoul2, 0, vehCent->m_pVehicle->m_pVehicleInfo->turret[turretNum].yawBone );
							}
							else if ( vehCent->m_pVehicle->m_pVehicleInfo->turret[turretNum].pitchBone )
							{
								boltIndex = trap_G2API_AddBolt( vehCent->ghoul2, 0, vehCent->m_pVehicle->m_pVehicleInfo->turret[turretNum].pitchBone );
							}
							else
							{//well, no way of knowing, so screw it
								return qfalse;
							}
						}
						if ( boltIndex != -1 )
						{
							mdxaBone_t boltMatrix;
							vec3_t fwd, up;
							trap_G2API_GetBoltMatrix_NoRecNoRot(vehCent->ghoul2, 0, boltIndex, &boltMatrix, vehCent->lerpAngles,
								vehCent->lerpOrigin, cg.time, NULL, vehCent->modelScale);
							BG_GiveMeVectorFromMatrix(&boltMatrix, ORIGIN, cg.refdef.vieworg);
							if ( hackPosAndAngle )
							{
								//FIXME: these are assumptions, externalize?  BETTER YET: give me a controller view bolt/tag for each turret
								BG_GiveMeVectorFromMatrix(&boltMatrix, NEGATIVE_X, fwd);
								BG_GiveMeVectorFromMatrix(&boltMatrix, NEGATIVE_Y, up);
								VectorMA( cg.refdef.vieworg, 8.0f, fwd, cg.refdef.vieworg );
								VectorMA( cg.refdef.vieworg, 4.0f, up, cg.refdef.vieworg );
							}
							else
							{
								BG_GiveMeVectorFromMatrix(&boltMatrix, NEGATIVE_Y, fwd);
							}
							{
								vec3_t	newAngles, deltaAngles;
								vectoangles( fwd, newAngles );
								AnglesSubtract( newAngles, cg_lastTurretViewAngles, deltaAngles );
								VectorMA( cg_lastTurretViewAngles, 0.5f*(float)cg.frametime/100.0f, deltaAngles, cg.refdef.viewangles );
							}
							return qtrue;
						}
					}
				}
			}
		}
	}
	return qfalse;
}
/*
===============
CG_CalcViewValues

Sets cg.refdef view values
===============
*/
void CG_EmplacedView(vec3_t angles);
static int CG_CalcViewValues( void ) {
	qboolean manningTurret = qfalse;
	playerState_t	*ps;

	memset( &cg.refdef, 0, sizeof( cg.refdef ) );

	// strings for in game rendering
	// Q_strncpyz( cg.refdef.text[0], "Park Ranger", sizeof(cg.refdef.text[0]) );
	// Q_strncpyz( cg.refdef.text[1], "19", sizeof(cg.refdef.text[1]) );

	// calculate size of 3D view
	CG_CalcVrect();

	ps = &cg.predictedPlayerState;
	//if(cg.demoPlayback&&pug_demospec.integer!=-1) ps = cg_entities[pug_demospec.integer].playerState;
/*
	if (cg.cameraMode) {
		vec3_t origin, angles;
		if (trap_getCameraInfo(cg.time, &origin, &angles)) {
			VectorCopy(origin, cg.refdef.vieworg);
			angles[ROLL] = 0;
			VectorCopy(angles, cg.refdef.viewangles);
			AnglesToAxis( cg.refdef.viewangles, cg.refdef.viewaxis );
			return CG_CalcFov();
		} else {
			cg.cameraMode = qfalse;
		}
	}
*/
	// intermission view
	if ( ps->pm_type == PM_INTERMISSION ) {
		VectorCopy( ps->origin, cg.refdef.vieworg );
		VectorCopy( ps->viewangles, cg.refdef.viewangles );
		AnglesToAxis( cg.refdef.viewangles, cg.refdef.viewaxis );
		return CG_CalcFov();
	}

	cg.bobcycle = ( ps->bobCycle & 128 ) >> 7;
	cg.bobfracsin = fabs( sin( ( ps->bobCycle & 127 ) / 127.0 * M_PI ) );
	cg.xyspeed = sqrt( ps->velocity[0] * ps->velocity[0] +
		ps->velocity[1] * ps->velocity[1] );

	if (cg.xyspeed > 270)
	{
		cg.xyspeed = 270;
	}

	manningTurret = CG_CheckPassengerTurretView();
	if ( !manningTurret )
	{//not manning a turret on a vehicle
		VectorCopy( ps->origin, cg.refdef.vieworg );
#ifdef VEH_CONTROL_SCHEME_4
		if ( cg.predictedPlayerState.m_iVehicleNum )//in a vehicle
		{
			Vehicle_t *pVeh = cg_entities[cg.predictedPlayerState.m_iVehicleNum].m_pVehicle;
			if ( BG_UnrestrainedPitchRoll( &cg.predictedPlayerState, pVeh ) )//can roll/pitch without restriction
			{//use the vehicle's viewangles to render view!
				VectorCopy( cg.predictedVehicleState.viewangles, cg.refdef.viewangles );
			}
			else if ( pVeh //valid vehicle data pointer
				&& pVeh->m_pVehicleInfo//valid vehicle info
				&& pVeh->m_pVehicleInfo->type == VH_FIGHTER )//fighter
			{
				VectorCopy( cg.predictedVehicleState.viewangles, cg.refdef.viewangles );
				cg.refdef.viewangles[PITCH] = AngleNormalize180( cg.refdef.viewangles[PITCH] );
			}
			else
			{
				VectorCopy( ps->viewangles, cg.refdef.viewangles );
			}
		}
#else// VEH_CONTROL_SCHEME_4
		if ( cg.predictedPlayerState.m_iVehicleNum //in a vehicle
			&& BG_UnrestrainedPitchRoll( &cg.predictedPlayerState, cg_entities[cg.predictedPlayerState.m_iVehicleNum].m_pVehicle ) )//can roll/pitch without restriction
		{//use the vehicle's viewangles to render view!
			VectorCopy( cg.predictedVehicleState.viewangles, cg.refdef.viewangles );
		}
#endif// VEH_CONTROL_SCHEME_4
		else
		{
			VectorCopy( ps->viewangles, cg.refdef.viewangles );
		}
	}
	VectorCopy( cg.refdef.viewangles, cg_lastTurretViewAngles );

	if (cg_cameraOrbit.integer) {
		if (cg.time > cg.nextOrbitTime) {
			cg.nextOrbitTime = cg.time + cg_cameraOrbitDelay.integer;
			cg_thirdPersonAngle.value += cg_cameraOrbit.value;
		}
	}
	// add error decay
	if ( cg_errorDecay.value > 0 ) {
		int		t;
		float	f;

		t = cg.time - cg.predictedErrorTime;
		f = ( cg_errorDecay.value - t ) / cg_errorDecay.value;
		if ( f > 0 && f < 1 ) {
			VectorMA( cg.refdef.vieworg, f, cg.predictedError, cg.refdef.vieworg );
		} else {
			cg.predictedErrorTime = 0;
		}
	}

	if (cg.snap->ps.weapon == WP_EMPLACED_GUN &&
		cg.snap->ps.emplacedIndex)
	{ //constrain the view properly for emplaced guns
		CG_EmplacedView(cg_entities[cg.snap->ps.emplacedIndex].currentState.angles);
	}

	if ( !manningTurret )
	{
		if ( cg.predictedPlayerState.m_iVehicleNum //in a vehicle
			&& BG_UnrestrainedPitchRoll( &cg.predictedPlayerState, cg_entities[cg.predictedPlayerState.m_iVehicleNum].m_pVehicle ) )//can roll/pitch without restriction
		{//use the vehicle's viewangles to render view!
			CG_OffsetFighterView();
		}
		else if ( cg.renderingThirdPerson ) {
			// back away from character
			if (cg_thirdPersonSpecialCam.integer &&
				BG_SaberInSpecial(cg.snap->ps.saberMove))
			{ //the action cam
				if (!CG_ThirdPersonActionCam())
				{ //couldn't do it for whatever reason, resort back to third person then
					CG_OffsetThirdPersonView();
				}
			}
			else
			{
				CG_OffsetThirdPersonView();
			}
		} else {
			// offset for local bobbing and kicks
			CG_OffsetFirstPersonView();
		}
	}

	// position eye relative to origin
	AnglesToAxis( cg.refdef.viewangles, cg.refdef.viewaxis );

	if ( cg.hyperspace ) {
		cg.refdef.rdflags |= RDF_NOWORLDMODEL | RDF_HYPERSPACE;
	}

	// field of view
	return CG_CalcFov();
}


/*
=====================
CG_PowerupTimerSounds
=====================
*/
static void CG_PowerupTimerSounds( void ) {
	int		i;
	int		t;

	// powerup timers going away
	for ( i = 0 ; i < MAX_POWERUPS ; i++ ) {
		t = cg.snap->ps.powerups[i];
		if ( t <= cg.time ) {
			continue;
		}
		if ( t - cg.time >= POWERUP_BLINKS * POWERUP_BLINK_TIME ) {
			continue;
		}
		if ( ( t - cg.time ) / POWERUP_BLINK_TIME != ( t - cg.oldTime ) / POWERUP_BLINK_TIME ) {
			//trap_S_StartSound( NULL, cg.snap->ps.clientNum, CHAN_ITEM, cgs.media.wearOffSound );
		}
	}
}

/*
==============
CG_DrawSkyBoxPortal
==============
*/
extern qboolean cg_skyOri;
extern vec3_t cg_skyOriPos;
extern float cg_skyOriScale;
extern qboolean cg_noFogOutsidePortal;
void CG_DrawSkyBoxPortal(const char *cstr)
{
	static float lastfov;
	refdef_t backuprefdef;
	float fov_x;
	float fov_y;
	float x;
	char *token;
	float f = 0;

	lastfov = zoomFov;	// for transitions back from zoomed in modes

	backuprefdef = cg.refdef;

	token = COM_ParseExt(&cstr, qfalse);
	if (!token || !token[0])
	{
		CG_Error( "CG_DrawSkyBoxPortal: error parsing skybox configstring\n");
	}
	cg.refdef.vieworg[0] = atof(token);

	token = COM_ParseExt(&cstr, qfalse);
	if (!token || !token[0])
	{
		CG_Error( "CG_DrawSkyBoxPortal: error parsing skybox configstring\n");
	}
	cg.refdef.vieworg[1] = atof(token);

	token = COM_ParseExt(&cstr, qfalse);
	if (!token || !token[0])
	{
		CG_Error( "CG_DrawSkyBoxPortal: error parsing skybox configstring\n");
	}
	cg.refdef.vieworg[2] = atof(token);

	token = COM_ParseExt(&cstr, qfalse);
	if (!token || !token[0]) 
	{
		CG_Error( "CG_DrawSkyBoxPortal: error parsing skybox configstring\n");
	}
	fov_x = atoi(token);

	if (!fov_x)
	{
		fov_x = cg_fov.value;
	}

	// setup fog the first time, ignore this part of the configstring after that
	token = COM_ParseExt(&cstr, qfalse);
	if (!token || !token[0])
	{
		CG_Error( "CG_DrawSkyBoxPortal: error parsing skybox configstring.  No fog state\n");
	}
	else 
	{
		vec4_t	fogColor;
		int		fogStart, fogEnd;

		if(atoi(token))
		{	// this camera has fog
			token = COM_ParseExt(&cstr, qfalse);

			if (!token || !token[0])
			{
				CG_Error( "CG_DrawSkyBoxPortal: error parsing skybox configstring.  No fog[0]\n");
			}
			fogColor[0] = atof(token);

			token = COM_ParseExt(&cstr, qfalse);
			if (!token || !token[0])
			{
				CG_Error( "CG_DrawSkyBoxPortal: error parsing skybox configstring.  No fog[1]\n");
			}
			fogColor[1] = atof(token);

			token = COM_ParseExt(&cstr, qfalse);
			if (!token || !token[0])
			{
				CG_Error( "CG_DrawSkyBoxPortal: error parsing skybox configstring.  No fog[2]\n");
			}
			fogColor[2] = atof(token);

			token = COM_ParseExt(&cstr, qfalse);
			if (!token || !token[0])
			{
				fogStart = 0;
			}
			else
			{
				fogStart = atoi(token);
			}

			token = COM_ParseExt(&cstr, qfalse);
			if (!token || !token[0])
			{
				fogEnd = 0;
			}
			else
			{
				fogEnd = atoi(token);
			}
		}
	}

	if ( cg.predictedPlayerState.pm_type == PM_INTERMISSION )
	{
		// if in intermission, use a fixed value
		fov_x = cg_fov.value;
	}
	else
	{
		fov_x = cg_fov.value;
		if ( fov_x < 1 ) 
		{
			fov_x = 1;
		}
		else if ( fov_x > 160 )
		{
			fov_x = 160;
		}

		if (cg.predictedPlayerState.zoomMode)
		{
			fov_x = zoomFov;
		}

		// do smooth transitions for zooming
		if (cg.predictedPlayerState.zoomMode)
		{ //zoomed/zooming in
			f = ( cg.time - cg.zoomTime ) / (float)ZOOM_OUT_TIME;
			if ( f > 1.0 ) {
				fov_x = zoomFov;
			} else {
				fov_x = fov_x + f * ( zoomFov - fov_x );
			}
			lastfov = fov_x;
		}
		else
		{ //zooming out
			f = ( cg.time - cg.zoomTime ) / (float)ZOOM_OUT_TIME;
			if ( f > 1.0 ) {
				fov_x = fov_x;
			} else {
				fov_x = zoomFov + f * ( fov_x - zoomFov);
			}
		}
	}

	x = cg.refdef.width / tan( fov_x / 360 * M_PI );
	fov_y = atan2( cg.refdef.height, x );
	fov_y = fov_y * 360 / M_PI;

	cg.refdef.fov_x = fov_x;
	cg.refdef.fov_y = fov_y;
	
	cg.refdef.rdflags |= RDF_SKYBOXPORTAL;
	cg.refdef.rdflags |= RDF_DRAWSKYBOX;

	cg.refdef.time = cg.time;

	if ( !cg.hyperspace) 
	{ //rww - also had to add this to add effects being rendered in portal sky areas properly.
		trap_FX_AddScheduledEffects(qtrue);
	}

	CG_AddPacketEntities(qtrue); //rww - There was no proper way to put real entities inside the portal view before.
									//This will put specially flagged entities in the render.

	if (cg_skyOri)
	{ //ok, we want to orient the sky refdef vieworg based on the normal vieworg's relation to the ori pos
		vec3_t dif;

		VectorSubtract(backuprefdef.vieworg, cg_skyOriPos, dif);
		VectorScale(dif, cg_skyOriScale, dif);
		VectorAdd(cg.refdef.vieworg, dif, cg.refdef.vieworg);
	}

	if (cg_noFogOutsidePortal)
	{ //make sure no fog flag is stripped first, and make sure it is set on the normal refdef
		cg.refdef.rdflags &= ~RDF_NOFOG;
		backuprefdef.rdflags |= RDF_NOFOG;
	}

	// draw the skybox
	trap_R_RenderScene( &cg.refdef );

	cg.refdef = backuprefdef;
}

/*
=====================
CG_AddBufferedSound
=====================
*/
void CG_AddBufferedSound( sfxHandle_t sfx ) {
	if ( !sfx )
		return;
	cg.soundBuffer[cg.soundBufferIn] = sfx;
	cg.soundBufferIn = (cg.soundBufferIn + 1) % MAX_SOUNDBUFFER;
	if (cg.soundBufferIn == cg.soundBufferOut) {
		cg.soundBufferOut++;
	}
}

/*
=====================
CG_PlayBufferedSounds
=====================
*/
static void CG_PlayBufferedSounds( void ) {
	if ( cg.soundTime < cg.time ) {
		if (cg.soundBufferOut != cg.soundBufferIn && cg.soundBuffer[cg.soundBufferOut]) {
			trap_S_StartLocalSound(cg.soundBuffer[cg.soundBufferOut], CHAN_ANNOUNCER);
			cg.soundBuffer[cg.soundBufferOut] = 0;
			cg.soundBufferOut = (cg.soundBufferOut + 1) % MAX_SOUNDBUFFER;
			cg.soundTime = cg.time + 750;
		}
	}
}

void CG_UpdateSoundTrackers()
{
	int num;
	centity_t *cent;

	for ( num = 0 ; num < ENTITYNUM_NONE ; num++ )
	{
		cent = &cg_entities[num];

		if (cent && (cent->currentState.eFlags & EF_SOUNDTRACKER) && cent->currentState.number == num)
			//make sure the thing is valid at least.
		{ //keep sound for this entity updated in accordance with its attached entity at all times
			if (cg.snap && cent->currentState.trickedentindex == cg.snap->ps.clientNum)
			{ //this is actually the player, so center the sound origin right on top of us
				VectorCopy(cg.refdef.vieworg, cent->lerpOrigin);
				trap_S_UpdateEntityPosition( cent->currentState.number, cent->lerpOrigin );
			}
			else
			{
				trap_S_UpdateEntityPosition( cent->currentState.number, cg_entities[cent->currentState.trickedentindex].lerpOrigin );
			}
		}

		if (cent->currentState.number == num)
		{
			//update all looping sounds..
			CG_S_UpdateLoopingSounds(num);
		}
	}
}

//=========================================================================

/*
================================
Screen Effect stuff starts here
================================
*/
#define	CAMERA_DEFAULT_FOV			90.0f
#define MAX_SHAKE_INTENSITY			16.0f

cgscreffects_t cgScreenEffects;

void CG_SE_UpdateShake( vec3_t origin, vec3_t angles )
{
	vec3_t	moveDir;
	float	intensity_scale, intensity;
	int		i;
	
	if ( pug_demospec.integer!=-1) return;

	if ( cgScreenEffects.shake_duration <= 0 )
		return;

	if ( cg.time > ( cgScreenEffects.shake_start + cgScreenEffects.shake_duration ) )
	{
		cgScreenEffects.shake_intensity = 0;
		cgScreenEffects.shake_duration = 0;
		cgScreenEffects.shake_start = 0;
		return;
	}

	cgScreenEffects.FOV = CAMERA_DEFAULT_FOV;
	cgScreenEffects.FOV2 = CAMERA_DEFAULT_FOV;

	//intensity_scale now also takes into account FOV with 90.0 as normal
	intensity_scale = 1.0f - ( (float) ( cg.time - cgScreenEffects.shake_start ) / (float) cgScreenEffects.shake_duration ) * (((cgScreenEffects.FOV+cgScreenEffects.FOV2)/2.0f)/90.0f);

	intensity = cgScreenEffects.shake_intensity * intensity_scale;

	for ( i = 0; i < 3; i++ )
	{
		moveDir[i] = ( crandom() * intensity );
	}

	//Move the camera
	VectorAdd( origin, moveDir, origin );

	for ( i=0; i < 2; i++ ) // Don't do ROLL
		moveDir[i] = ( crandom() * intensity );

	//Move the angles
	VectorAdd( angles, moveDir, angles );
}

void CG_SE_UpdateMusic(void)
{
	if (cgScreenEffects.music_volume_multiplier < 0.1)
	{
		cgScreenEffects.music_volume_multiplier = 1.0;
		return;
	}

	if (cgScreenEffects.music_volume_time < cg.time)
	{
		if (cgScreenEffects.music_volume_multiplier != 1.0 || cgScreenEffects.music_volume_set)
		{
			char musMultStr[512];

			cgScreenEffects.music_volume_multiplier += 0.1;
			if (cgScreenEffects.music_volume_multiplier > 1.0)
			{
				cgScreenEffects.music_volume_multiplier = 1.0;
			}

			Com_sprintf(musMultStr, sizeof(musMultStr), "%f", cgScreenEffects.music_volume_multiplier);
			trap_Cvar_Set("s_musicMult", musMultStr);

			if (cgScreenEffects.music_volume_multiplier == 1.0)
			{
				cgScreenEffects.music_volume_set = qfalse;
			}
			else
			{
				cgScreenEffects.music_volume_time = cg.time + 200;
			}
		}

		return;
	}

	if (!cgScreenEffects.music_volume_set)
	{ //if the volume_time is >= cg.time, we should have a volume multiplier set
		char musMultStr[512];

		Com_sprintf(musMultStr, sizeof(musMultStr), "%f", cgScreenEffects.music_volume_multiplier);
		trap_Cvar_Set("s_musicMult", musMultStr);
		cgScreenEffects.music_volume_set = qtrue;
	}
}

/*
=================
CG_CalcScreenEffects

Currently just for screen shaking (and music volume management)
=================
*/
void CG_CalcScreenEffects(void)
{
	CG_SE_UpdateShake(cg.refdef.vieworg, cg.refdef.viewangles);
	CG_SE_UpdateMusic();
}

void CGCam_Shake( float intensity, int duration )
{
	if ( pug_demospec.integer!=-1||pug_demos_cameraMode.integer>=4) return;
	if ( intensity > MAX_SHAKE_INTENSITY )
		intensity = MAX_SHAKE_INTENSITY;

	cgScreenEffects.shake_intensity = intensity;
	cgScreenEffects.shake_duration = duration;
	

	cgScreenEffects.shake_start = cg.time;
//JLFRUMBLE
#ifdef _XBOX
extern void FF_XboxShake(float intensity, int duration);

FF_XboxShake(intensity, duration);

#endif
}

void CG_DoCameraShake( vec3_t origin, float intensity, int radius, int time )
{
	//FIXME: When exactly is the vieworg calculated in relation to the rest of the frame?s

	vec3_t	dir;
	float	dist, intensityScale;
	float	realIntensity;
	
	if( pug_demospec.integer!=-1||pug_demos_cameraMode.integer>=4) return;

	VectorSubtract( cg.refdef.vieworg, origin, dir );
	dist = VectorNormalize( dir );

	//Use the dir to add kick to the explosion

	if ( dist > radius )
		return;

	intensityScale = 1 - ( dist / (float) radius );
	realIntensity = intensity * intensityScale;

	CGCam_Shake( realIntensity, time );
}

void CGCam_SetMusicMult( float multiplier, int duration )
{
	if (multiplier < 0.1f)
	{
		multiplier = 0.1f;
	}

	if (multiplier > 1.0f)
	{
		multiplier = 1.0f;
	}

	cgScreenEffects.music_volume_multiplier = multiplier;
	cgScreenEffects.music_volume_time = cg.time + duration;
	cgScreenEffects.music_volume_set = qfalse;
}

/*
================================
Screen Effect stuff ends here
================================
*/

/*
=================
CG_EmplacedView

Keep view reasonably constrained in relation to gun -rww
=================
*/
#include "../namespace_begin.h"
int BG_EmplacedView(vec3_t baseAngles, vec3_t angles, float *newYaw, float constraint);
#include "../namespace_end.h"

void CG_EmplacedView(vec3_t angles)
{
	float yaw;
	int override;
	
	override = BG_EmplacedView(cg.refdef.viewangles, angles, &yaw,
		cg_entities[cg.snap->ps.emplacedIndex].currentState.origin2[0]);
	
	if (override)
	{
        cg.refdef.viewangles[YAW] = yaw;
		AnglesToAxis(cg.refdef.viewangles, cg.refdef.viewaxis);

		if (override == 2)
		{
			trap_SetClientForceAngle(cg.time + 5000, cg.refdef.viewangles);
		}
	}

	//we want to constrain the predicted player state viewangles as well
	override = BG_EmplacedView(cg.predictedPlayerState.viewangles, angles, &yaw,
		cg_entities[cg.snap->ps.emplacedIndex].currentState.origin2[0]);
	if (override)
	{
        cg.predictedPlayerState.viewangles[YAW] = yaw;
	}
}

//specially add cent's for automap
static void CG_AddRefentForAutoMap(centity_t *cent)
{
	refEntity_t ent;
	vec3_t flat;

	if (cent->currentState.eFlags & EF_NODRAW)
	{
		return;
	}

	memset(&ent, 0, sizeof(refEntity_t));
	ent.reType = RT_MODEL;

	VectorCopy(cent->lerpAngles, flat);
	flat[PITCH] = flat[ROLL] = 0.0f;

	VectorCopy(cent->lerpOrigin, ent.origin);
	VectorCopy(flat, ent.angles);
	AnglesToAxis(flat, ent.axis);

	if (cent->ghoul2 &&
		(cent->currentState.eType == ET_PLAYER ||
		cent->currentState.eType == ET_NPC ||
		cent->currentState.modelGhoul2))
	{ //using a ghoul2 model
		ent.ghoul2 = cent->ghoul2;
		ent.radius = cent->currentState.g2radius;

		if (!ent.radius)
		{
			ent.radius = 64.0f;
		}
	}
	else
	{ //then assume a standard indexed model
		ent.hModel = cgs.gameModels[cent->currentState.modelindex];
	}

	trap_R_AddRefEntityToScene(&ent);
}

//add all entities that would be on the radar
void CG_AddRadarAutomapEnts(void)
{
	int i = 0;

	//first add yourself
	CG_AddRefentForAutoMap(&cg_entities[cg.predictedPlayerState.clientNum]);

	while (i < cg.radarEntityCount)
	{
		CG_AddRefentForAutoMap(&cg_entities[cg.radarEntities[i]]);
		i++;
	}
}

/*
================
CG_DrawAutoMap

Draws the automap scene. -rww
================
*/
float cg_autoMapZoom = 512.0f;
float cg_autoMapZoomMainOffset = 0.0f;
vec3_t cg_autoMapAngle = {90.0f, 0.0f, 0.0f};
autoMapInput_t cg_autoMapInput;
int cg_autoMapInputTime = 0;
#define	SIDEFRAME_WIDTH			16
#define	SIDEFRAME_HEIGHT		32
void CG_DrawAutoMap(void)
{
	clientInfo_t	*local;
	refdef_t		refdef;
	trace_t			tr;
	vec3_t			fwd;
	vec3_t			playerMins, playerMaxs;
	int				vWidth, vHeight;
	float			hScale, vScale;
	float			x, y, w, h;

	if (!cg_autoMap.integer)
	{ //don't do anything then
		return;
	}

	if ( cg.snap->ps.stats[STAT_HEALTH] <= 0 )
	{ //don't show when dead
		return;
	}

	if ( (cg.predictedPlayerState.pm_flags & PMF_FOLLOW) || cg.predictedPlayerState.persistant[PERS_TEAM] == TEAM_SPECTATOR )
	{ //don't show when spec
		return;
	}

	local = &cgs.clientinfo[ cg.predictedPlayerState.clientNum ];
	if ( !local->infoValid )
	{ //don't show if bad ci
		return;
	}

	if (cgs.gametype < GT_TEAM)
	{ //don't show in non-team gametypes
		return;
	}

	if (cg_autoMapInputTime >= cg.time)
	{
		if (cg_autoMapInput.up)
		{
			cg_autoMapZoom -= cg_autoMapInput.up;
			if (cg_autoMapZoom < cg_autoMapZoomMainOffset+64.0f)
			{
				cg_autoMapZoom = cg_autoMapZoomMainOffset+64.0f;
			}
		}

		if (cg_autoMapInput.down)
		{
			cg_autoMapZoom += cg_autoMapInput.down;
			if (cg_autoMapZoom > cg_autoMapZoomMainOffset+4096.0f)
			{
				cg_autoMapZoom = cg_autoMapZoomMainOffset+4096.0f;
			}
		}

		if (cg_autoMapInput.yaw)
		{
			cg_autoMapAngle[YAW] += cg_autoMapInput.yaw;
		}

		if (cg_autoMapInput.pitch)
		{
			cg_autoMapAngle[PITCH] += cg_autoMapInput.pitch;
		}

		if (cg_autoMapInput.goToDefaults)
		{
			cg_autoMapZoom = 512.0f;
			VectorSet(cg_autoMapAngle, 90.0f, 0.0f, 0.0f);
		}
	}

	memset( &refdef, 0, sizeof( refdef ) );

	refdef.rdflags = (RDF_NOWORLDMODEL|RDF_AUTOMAP);

	VectorCopy(cg.predictedPlayerState.origin, refdef.vieworg);
	VectorCopy(cg_autoMapAngle, refdef.viewangles);
	
	//scale out in the direction of the view angles base on the zoom factor
	AngleVectors(refdef.viewangles, fwd, 0, 0);
	VectorMA(refdef.vieworg, -cg_autoMapZoom, fwd, refdef.vieworg);

	AnglesToAxis(refdef.viewangles, refdef.viewaxis);

	refdef.fov_x = 50;
	refdef.fov_y = 50;

	//guess this doesn't need to be done every frame, but eh
	trap_R_GetRealRes(&vWidth, &vHeight);

	//set scaling values so that the 640x480 will result at 1.0/1.0
	hScale = vWidth/640.0f;
	vScale = vHeight/480.0f;

	x = cg_autoMapX.value;
	y = cg_autoMapY.value;
	w = cg_autoMapW.value;
	h = cg_autoMapH.value;

	refdef.x = x*hScale;
	refdef.y = y*vScale;
	refdef.width = w*hScale;
	refdef.height = h*vScale;

	CG_DrawPic(x-SIDEFRAME_WIDTH, y, SIDEFRAME_WIDTH, h, cgs.media.wireframeAutomapFrame_left);
	CG_DrawPic(x+w, y, SIDEFRAME_WIDTH, h, cgs.media.wireframeAutomapFrame_right);
	CG_DrawPic(x-SIDEFRAME_WIDTH, y-SIDEFRAME_HEIGHT, w+(SIDEFRAME_WIDTH*2), SIDEFRAME_HEIGHT, cgs.media.wireframeAutomapFrame_top);
	CG_DrawPic(x-SIDEFRAME_WIDTH, y+h, w+(SIDEFRAME_WIDTH*2), SIDEFRAME_HEIGHT, cgs.media.wireframeAutomapFrame_bottom);

	refdef.time = cg.time;

	trap_R_ClearScene();
	CG_AddRadarAutomapEnts();

	if (cg.predictedPlayerState.m_iVehicleNum &&
		cg_entities[cg.predictedPlayerState.m_iVehicleNum].currentState.eType == ET_NPC &&
		cg_entities[cg.predictedPlayerState.m_iVehicleNum].currentState.NPC_class == CLASS_VEHICLE &&
		cg_entities[cg.predictedPlayerState.m_iVehicleNum].m_pVehicle &&
		cg_entities[cg.predictedPlayerState.m_iVehicleNum].m_pVehicle->m_pVehicleInfo->type == VH_FIGHTER)
	{ //constantly adjust to current height
		trap_R_AutomapElevAdj(cg.predictedPlayerState.origin[2]);
	}
	else
	{
		//Trace down and set the ground elevation as the main automap elevation point
		VectorSet(playerMins, -15, -15, DEFAULT_MINS_2);
		VectorSet(playerMaxs, 15, 15, DEFAULT_MAXS_2);

		VectorCopy(cg.predictedPlayerState.origin, fwd);
		fwd[2] -= 4096.0f;
		CG_Trace(&tr, cg.predictedPlayerState.origin, playerMins, playerMaxs, fwd, cg.predictedPlayerState.clientNum, MASK_SOLID);

		if (!tr.startsolid && !tr.allsolid)
		{
			trap_R_AutomapElevAdj(tr.endpos[2]);
		}
	}
	trap_R_RenderScene( &refdef );
}

/*
=================
CG_DrawActiveFrame

Generates and draws a game scene and status information at the given time.
=================
*/
static qboolean cg_rangedFogging = qfalse; //so we know if we should go back to normal fog
float cg_linearFogOverride = 0.0f; //designer-specified override for linear fogging style

#include "../namespace_begin.h"
extern void BG_VehicleTurnRateForSpeed( Vehicle_t *pVeh, float speed, float *mPitchOverride, float *mYawOverride );
extern qboolean PM_InKnockDown( playerState_t *ps );
#include "../namespace_end.h"

typedef struct {
	vec3_t		origin;
	vec3_t		angle;
	int			time;
	qboolean	thirdPerson;
	vec4_t		csi_coeff[6];   //cubic spline interpolation coefficients
} demoCam_t;

typedef struct {
	double		coefficient[3];
	vec4_t		csi_coeff;   //cubic spline interpolation coefficients for time
} newCam_t;

extern qboolean cgQueueLoad;
extern int gotime;
extern demoCam_t cam[50];
extern newCam_t newcam[50];
extern int curCam;
extern void CG_ActualLoadDeferredPlayers( void );
void drawSplines( void );
int camPreviewTime = 0;

static int cg_siegeClassIndex = -2;
static void swapPlayerstates( ) {
	playerState_t tmp;
	tmp = cg.predictedPlayerState;
	cg.predictedPlayerState = cg.truePredictedPlayerState;
	cg.truePredictedPlayerState = tmp;
}

#include <math.h>
float computePosition(int axis, double time) {
	double position = 0;
	int i;
	for (i = 0; i < curCam; i++) {
		position += pow(1 - time, (curCam - 1) - i) * pow(time, i) * newcam[i].coefficient[axis];
	}
	return position;
}

void CG_DrawActiveFrame( int serverTime, stereoFrame_t stereoView, qboolean demoPlayback ) {
	int		inwater,orgState;
	const char *cstr;
	float mSensitivity = cg.zoomSensitivity;
	float mPitchOverride = 0.0f;
	float mYawOverride = 0.0f;
	static centity_t *veh = NULL;
	static int oldTime = 0;
	static int avO1 = 0, avO2 = 0;
	snapshot_t snap, *oldSnap;
	//int mypw[MAX_POWERUPS];
	playerState_t prps;
	static qboolean demoCamming = qfalse, TPdemoCamming = qfalse, capDemoCamming = qfalse;
	static int camtimeStart = 0;
#ifdef VEH_CONTROL_SCHEME_4
	float mSensitivityOverride = 0.0f;
	qboolean bUseFighterPitch = qfalse;
	qboolean	isFighter = qfalse;
#endif
	qboolean isCapture = qfalse;
	static vec3_t	nextVieworg,nextViewangles;
	extern FILE *ghostFile;
	extern qboolean ghostMode;
	isCapture = ( Q_strncmp( "recordonly", pug_demos_cameraMode.string, 10 ) == 0 );
	
	if(pug_demos_cameraMode.integer == 4 || pug_demos_cameraMode.integer == 5 || pug_demos_cameraMode.integer == 6 || pug_demos_cameraMode.integer == 7 || Q_strncmp( "recordonly", pug_demos_cameraMode.string, 10 ) == 0 || gotime != 0) {
		if(demoPlayback && gotime!=0) {
			int nextTime;
			if(oldTime!=0)nextTime = (serverTime - cgs.levelStartTime) + ((serverTime - cgs.levelStartTime) - oldTime);
			else nextTime = serverTime - cgs.levelStartTime;
			oldTime = serverTime - cgs.levelStartTime;
			if( gotime - nextTime <= 0 ) {
				trap_Cvar_Set("timescale","1");
				gotime = oldTime = 0;
			} else {
				trap_Cvar_Set("timescale",va("%d",(1 + (gotime - nextTime) / 100)));
				//return;
			}
		} else if(demoPlayback||camPreviewTime) {
			double timeDelta;
			vec3_t curVec,curVeca;
			int i;
			double l_time=((double)(serverTime - cgs.levelStartTime))/1000;
			int myserverTime = 0;
			qboolean oldCamming;
			if( pug_demos_Timescalelesscams.integer && camtimeStart ) {camtimeStart += 100; myserverTime = camtimeStart;}
			else if( camPreviewTime ) myserverTime = ( serverTime - camPreviewTime ) + cam[0].time + cgs.levelStartTime;
			else myserverTime = serverTime;
			oldCamming = demoCamming || TPdemoCamming || capDemoCamming;
			demoCamming = capDemoCamming = qfalse;
			for(i = 0;i < curCam - 1;i++) {
				if(cam[i].time!=0 && cam[i].time <= myserverTime - cgs.levelStartTime && cam[i+1].time > myserverTime - cgs.levelStartTime && cam[i+1].time > cam[i].time){demoCamming = qtrue;break;}
			}
			if(demoCamming) {
				if( !isCapture )
				{
					if( pug_demos_Timescalelesscams.integer && !camtimeStart ) camtimeStart = /*trap_Milliseconds() - */serverTime;
					timeDelta = (double)(myserverTime - cgs.levelStartTime - cam[i].time) / (double)(cam[i+1].time - cam[i].time);
					l_time=((double)(myserverTime - cgs.levelStartTime - cam[i].time))/1000;
					if( cam[i].thirdPerson ) {
						demoCamming = qfalse; //we still need 2 do third person angles here...
						TPdemoCamming = qtrue;
						cg_thirdPersonHorzOffset.value = cam[i+1].origin[1] * timeDelta + cam[i].origin[1] * (1 - timeDelta);
						cg_thirdPersonVertOffset.value = cam[i+1].origin[2] * timeDelta + cam[i].origin[2] * (1 - timeDelta);
						cg_thirdPersonPitchOffset.value = cam[i+1].angle[0] * timeDelta + cam[i].angle[0] * (1 - timeDelta);
						cg_thirdPersonAngle.value = cam[i+1].angle[1] * timeDelta + cam[i].angle[1] * (1 - timeDelta);
						cg_thirdPersonRange.value = cam[i+1].angle[2] * timeDelta + cam[i].angle[2] * (1 - timeDelta);
						if(!oldCamming && cam_autorecord.integer>0)trap_Cvar_Set( "cl_avidemo", va( "%d", cam_autorecord.integer ) );
					} else {
						vec3_t atOrg, dir2Me;
						/*curVec[0] = cam[i+1].origin[0] * timeDelta + cam[i].origin[0] * (1 - timeDelta);
						curVec[1] = cam[i+1].origin[1] * timeDelta + cam[i].origin[1] * (1 - timeDelta);
						curVec[2] = cam[i+1].origin[2] * timeDelta + cam[i].origin[2] * (1 - timeDelta);*/

//						curVec[0] = cam[i].csi_coeff[0][0]*l_time*l_time*l_time+cam[i].csi_coeff[0][1]*l_time*l_time+cam[i].csi_coeff[0][2]*l_time+cam[i].csi_coeff[0][3];
//						curVec[1] = cam[i].csi_coeff[1][0]*l_time*l_time*l_time+cam[i].csi_coeff[1][1]*l_time*l_time+cam[i].csi_coeff[1][2]*l_time+cam[i].csi_coeff[1][3];
//						curVec[2] = cam[i].csi_coeff[2][0]*l_time*l_time*l_time+cam[i].csi_coeff[2][1]*l_time*l_time+cam[i].csi_coeff[2][2]*l_time+cam[i].csi_coeff[2][3];

						{
							double time = ((myserverTime - cgs.levelStartTime) - cam[i].time) / (double) (cam[curCam - 1].time - cam[0].time);
							time = newcam[i].csi_coeff[0]*time*time*time + newcam[i].csi_coeff[1]*time*time + newcam[i].csi_coeff[2]*time + newcam[i].csi_coeff[3];
							time += i * (1 / (double) (curCam - 1));
							curVec[0] = computePosition(0, time);
							curVec[1] = computePosition(1, time);
							curVec[2] = computePosition(2, time);
							CG_Printf("Time: %lf, NumCams=%d, Position = {x=%lf,y=%lf,z=%lf}\n", time, curCam, curVec[0], curVec[1], curVec[2]);
						}
//						VectorAdd( curVec, cam[i].origin, curVec );
						/*curVeca[0] = cam[i+1].angle[0] * timeDelta + cam[i].angle[0] * (1 - timeDelta);
						curVeca[1] = cam[i+1].angle[1] * timeDelta + cam[i].angle[1] * (1 - timeDelta);
						curVeca[2] = cam[i+1].angle[2] * timeDelta + cam[i].angle[2] * (1 - timeDelta);*/
						curVeca[0] = cam[i].csi_coeff[3][0]*l_time*l_time*l_time+cam[i].csi_coeff[3][1]*l_time*l_time+cam[i].csi_coeff[3][2]*l_time+cam[i].csi_coeff[3][3];
						curVeca[1] = cam[i].csi_coeff[4][0]*l_time*l_time*l_time+cam[i].csi_coeff[4][1]*l_time*l_time+cam[i].csi_coeff[4][2]*l_time+cam[i].csi_coeff[4][3];
						curVeca[2] = cam[i].csi_coeff[5][0]*l_time*l_time*l_time+cam[i].csi_coeff[5][1]*l_time*l_time+cam[i].csi_coeff[5][2]*l_time+cam[i].csi_coeff[5][3];
						VectorAdd( curVeca, cam[i].angle, curVeca );
						if(pug_demos_cameraMode.integer>4) {
							//point at current focused player
							VectorCopy( cg_entities[pug_demospec.integer!=-1?pug_demospec.integer:cg.predictedPlayerState.clientNum].lerpOrigin, atOrg );
							//atOrg[2] = curVec[2];
							VectorSubtract( atOrg, curVec, dir2Me );
							vectoangles( dir2Me, curVeca );
							curVeca[2] = cam[i].csi_coeff[5][0]*l_time*l_time*l_time+cam[i].csi_coeff[5][1]*l_time*l_time+cam[i].csi_coeff[5][2]*l_time+cam[i].csi_coeff[5][3];
						}
						if(pug_demos_cameraMode.integer == 6 )
							curVeca[PITCH] = cam[i].angle[PITCH] + cam[i].csi_coeff[3][0]*l_time*l_time*l_time+cam[i].csi_coeff[3][1]*l_time*l_time+cam[i].csi_coeff[3][2]*l_time+cam[i].csi_coeff[3][3];
						if(pug_demos_cameraMode.integer == 7 )
							curVeca[YAW] = cam[i].angle[YAW] + cam[i].csi_coeff[4][0]*l_time*l_time*l_time+cam[i].csi_coeff[4][1]*l_time*l_time+cam[i].csi_coeff[4][2]*l_time+cam[i].csi_coeff[4][3];
						VectorCopy( curVec, cg.refdef.vieworg );
			    	    VectorCopy( curVeca, cg.refdef.viewangles );
			        	AnglesToAxis( cg.refdef.viewangles, cg.refdef.viewaxis );
				        CG_CalcFov();
					}
				} else {
					demoCamming = qfalse;
					capDemoCamming = qtrue;
				}
				if(!oldCamming && cam_autosound.value>0)
				{
					trap_Cvar_Set( "s_volume", va( "%f", cam_autosound.value ) );
				}
		        if(!oldCamming && cam_autorecord.integer>0)
				{
					trap_Cvar_Set( "cl_avidemo", va( "%d", cam_autorecord.integer ) );
				} else if( cam_autorecord.integer > 0 ) {
					float curFps;
					int highFps;
					int lowFps;
					// count ms under/over
					// one more frame taken
					avO1++;
					// total msec so far
					avO2 += serverTime - cg.oldTime;
					// total real fps so far
					curFps = ((float)avO1) / (avO2/1000.0f);
					// fix up the fps by temporarily changing the avidemo value
					// first find the high and low values nearest to current desired avidemo
					// in order to gravitate better
					lowFps = (int)( 1000.0f / ceil( 1000.0f / cam_autorecord.integer ) );
					highFps = (int)( 1000.0f / (int)( 1000.0f / cam_autorecord.integer ) );
					if( curFps > cam_autorecord.integer ) trap_Cvar_Set( "cl_avidemo", va( "%d", lowFps ) );
					else if( curFps < cam_autorecord.integer ) trap_Cvar_Set( "cl_avidemo", va( "%d", highFps ) );
				}
	    	    //if(cam_endTime - (myserverTime - cgs.levelStartTime)<=0)cam_endTime = cam_startTime = 0;
			} else if(oldCamming) {
				if(cam_autorecord.integer>0) {
					trap_Cvar_Set( "cl_avidemo", "0" );
					avO1 = avO2 = 0;
				}
				if(cam_autosound.value>0) {
					trap_Cvar_Set( "s_volume", "0" );
				}
			} else if(camPreviewTime)
				camPreviewTime = 0;
		} else if(demoCamming||capDemoCamming)
			//but we're not!
			demoCamming = capDemoCamming = qfalse;
	}

	if (cgQueueLoad)
	{ //do this before you start messing around with adding ghoul2 refents and crap
		CG_ActualLoadDeferredPlayers();
		cgQueueLoad = qfalse;
	}

	cg.time = serverTime;
	cg.demoPlayback = demoPlayback;
    /*if( cg.demoPlayback && pug_demospec.integer!=-1 )
    {
        //cg.predictedPlayerState.clientNum = pug_demospec.integer;
        memcpy(&(cg.snap->ps),cg_entities[pug_demospec.integer].playerState,sizeof(playerState_t));
        memcpy(&(cg.nextSnap->ps),cg_entities[pug_demospec.integer].playerState,sizeof(playerState_t));
    }*/

	if (cg.snap && ui_myteam.integer != cg.snap->ps.persistant[PERS_TEAM])
	{
		trap_Cvar_Set ( "ui_myteam", va("%i", cg.snap->ps.persistant[PERS_TEAM]) );
	}
	if (cgs.gametype == GT_SIEGE &&
		cg.snap &&
		cg_siegeClassIndex != cgs.clientinfo[cg.snap->ps.clientNum].siegeIndex)
	{
		cg_siegeClassIndex = cgs.clientinfo[cg.snap->ps.clientNum].siegeIndex;
		if (cg_siegeClassIndex == -1)
		{
			trap_Cvar_Set("ui_mySiegeClass", "<none>");
		}
		else
		{
			trap_Cvar_Set("ui_mySiegeClass", bgSiegeClasses[cg_siegeClassIndex].name);
		}
	}

	// update cvars
	CG_UpdateCvars();

	// if we are only updating the screen as a loading
	// pacifier, don't even try to read snapshots
	if ( cg.infoScreenText[0] != 0 ) {
		CG_DrawInformation();
		return;
	}

	//fxT = cg.time;
	trap_FX_AdjustTime( cg.time /*/ (1000/fx_vfps.integer) * (1000/fx_vfps.integer)*/ );
	
	CG_RunLightStyles();

	// any looped sounds will be respecified as entities
	// are added to the render list
	trap_S_ClearLoopingSounds();

	// clear all the render lists
	trap_R_ClearScene();

	oldSnap = cg.nextSnap;
	// set up cg.snap and possibly cg.nextSnap
	CG_ProcessSnapshots();
	if( cg.demoPlayback && ( pug_demos_cameraMode.integer == 1 || pug_demos_cameraMode.integer == 2 || pug_demos_cameraMode.integer == 3 ) ) {
		cg.truePredictedPlayerState = cg.predictedPlayerState;
		cg.truePredictedPlayerState.pm_flags = 0;
		cg.truePredictedPlayerState.pm_type = PM_SPECTATOR;
	}

	trap_ROFF_UpdateEntities();

	// if we haven't received any snapshots yet, all
	// we can draw is the information screen
	if ( !cg.snap || ( cg.snap->snapFlags & SNAPFLAG_NOT_ACTIVE ) )
	{
#if 0	
		// Transition from zero to negative one on the snapshot timeout.
		// The reason we do this is because the first client frame is responsible for
		// some farily slow processing (such as weather) and we dont want to include
		// that processing time into our calculations
		if ( !cg.snapshotTimeoutTime )
		{
			cg.snapshotTimeoutTime = -1;
		}
		// Transition the snapshot timeout time from -1 to the current time in 
		// milliseconds which will start the timeout.
		else if ( cg.snapshotTimeoutTime == -1 )
		{		
			cg.snapshotTimeoutTime = trap_Milliseconds ( );
		}

		// If we have been waiting too long then just error out
		if ( cg.snapshotTimeoutTime > 0 && (trap_Milliseconds ( ) - cg.snapshotTimeoutTime > cg_snapshotTimeout.integer * 1000) )
		{
			Com_Error ( ERR_DROP, CG_GetStringEdString("MP_SVGAME", "SNAPSHOT_TIMEOUT"));
			return;
		}
#endif	
		CG_DrawInformation();
		return;
	}

	// let the client system know what our weapon and zoom settings are
	if (cg.snap && cg.snap->ps.saberLockTime > cg.time)
	{
		mSensitivity = 0.01f;
	}
	else if (cg.predictedPlayerState.weapon == WP_EMPLACED_GUN)
	{ //lower sens for emplaced guns and vehicles
		mSensitivity = 0.2f;
	}
#ifdef VEH_CONTROL_SCHEME_4
	else if (cg.predictedPlayerState.m_iVehicleNum//in a vehicle
		&& !cg.predictedPlayerState.generic1 )//not as a passenger
	{
		centity_t *cent = &cg_entities[cg.predictedPlayerState.m_iVehicleNum];
		if ( cent->m_pVehicle
			&& cent->m_pVehicle->m_pVehicleInfo
			&& cent->m_pVehicle->m_pVehicleInfo->type == VH_FIGHTER )
		{
			BG_VehicleTurnRateForSpeed( cent->m_pVehicle, cent->currentState.speed, &mPitchOverride, &mYawOverride );
			//mSensitivityOverride = 5.0f;//old default value
			mSensitivityOverride = 0.0f;
			bUseFighterPitch = qtrue;
			trap_SetUserCmdValue( cg.weaponSelect, mSensitivity, mPitchOverride, mYawOverride, mSensitivityOverride, cg.forceSelect, cg.itemSelect, bUseFighterPitch );
			isFighter = qtrue;
		}
	} 

	if ( !isFighter )
#endif //VEH_CONTROL_SCHEME_4
	{
		if (cg.predictedPlayerState.m_iVehicleNum)
		{
			veh = &cg_entities[cg.predictedPlayerState.m_iVehicleNum];
		}
		if (veh &&
			veh->currentState.eType == ET_NPC &&
			veh->currentState.NPC_class == CLASS_VEHICLE &&
			veh->m_pVehicle &&
			veh->m_pVehicle->m_pVehicleInfo->type == VH_FIGHTER &&
			bg_fighterAltControl.integer)
		{
			trap_SetUserCmdValue( cg.weaponSelect, mSensitivity, mPitchOverride, mYawOverride, 0.0f, cg.forceSelect, cg.itemSelect, qtrue );
			veh = NULL; //this is done because I don't want an extra assign each frame because I am so perfect and super efficient.
		}
		else
		{
			trap_SetUserCmdValue( cg.weaponSelect, mSensitivity, mPitchOverride, mYawOverride, 0.0f, cg.forceSelect, cg.itemSelect, qfalse );
		}
	}

	// this counter will be bumped for every valid scene we generate
	cg.clientFrame++;

	// update cg.predictedPlayerState
	if( pug_demos_cameraMode.integer ) {
		int oldCam;
		oldCam = pug_demos_cameraMode.integer;
		pug_demos_cameraMode.integer = 0;
		CG_PredictPlayerState();
		pug_demos_cameraMode.integer = oldCam;
		cg.snap->ps.zoomMode = cg.predictedPlayerState.zoomMode = 0;
	} else
		CG_PredictPlayerState();
	
	if( cg.demoPlayback && ( pug_demos_cameraMode.integer == 1 || pug_demos_cameraMode.integer == 2 || pug_demos_cameraMode.integer == 3 ) ) {
		playerState_t tmpswap,tmpswap2;
		centity_t plz2do;
		clientInfo_t plz2doci;
		int oldNum, oldFlags, oldType, oldTeam;
		snapshot_t *nextSnap;
		plz2do = cg_entities[cg.predictedPlayerState.clientNum];
		plz2doci = cgs.clientinfo[cg.predictedPlayerState.clientNum];
		oldNum = cg.snap->ps.clientNum;
		tmpswap = cg.predictedPlayerState;
		cg.predictedPlayerState = cg.truePredictedPlayerState;
		cg.truePredictedPlayerState = tmpswap;
		cg.snap->ps.clientNum = cg.nextSnap->ps.clientNum = cg.predictedPlayerState.clientNum = MAX_CLIENTS - 1;
		tmpswap = cg.snap->ps;
		tmpswap2 = cg.nextSnap->ps;
		cg.snap->ps = cg.nextSnap->ps = cg.predictedPlayerState;
		nextSnap = cg.nextSnap;
		cg.nextSnap = 0;
		oldFlags = cg.snap->ps.pm_flags;
		oldType = cg.snap->ps.pm_type;
		oldTeam = cg.snap->ps.persistant[PERS_TEAM];
		cg.snap->ps.pm_flags = 0;
		cg.snap->ps.pm_type = PM_SPECTATOR;
		cg.snap->ps.persistant[PERS_TEAM] = TEAM_SPECTATOR;
		CG_PredictPlayerState();
		cg.snap->ps.pm_flags = oldFlags;
		cg.snap->ps.pm_type = oldType;
		cg.snap->ps.persistant[PERS_TEAM] = oldTeam;
		cg.nextSnap = nextSnap;
		VectorCopy( cg.predictedPlayerState.origin, cg_entities[MAX_CLIENTS - 1].currentState.pos.trBase );
		VectorCopy( cg.predictedPlayerState.velocity, cg_entities[MAX_CLIENTS - 1].currentState.pos.trDelta );
		cg.snap->ps = tmpswap;
		cg.nextSnap->ps = tmpswap2;
		cg.snap->ps.clientNum = cg.nextSnap->ps.clientNum = oldNum;
		swapPlayerstates( );
		if( pug_demos_cameraMode.integer == 2 || pug_demos_cameraMode.integer == 3 ) {
			VectorCopy( cg.predictedPlayerState.origin, cg.truePredictedPlayerState.origin );
		}
		cg_entities[cg.predictedPlayerState.clientNum] = plz2do;
		cgs.clientinfo[cg.predictedPlayerState.clientNum] = plz2doci;
		cg.snap->ps.stats[STAT_HEALTH] = cg.snap->ps.stats[STAT_MAX_HEALTH];
	}
	
	if( pug_demos_cameraMode.integer > 2 || ui_noScope.integer )
		cg.snap->ps.zoomMode = cg.predictedPlayerState.zoomMode = zoomFov = 0;
	
	/*if( cg.demoPlayback && pug_demospec.integer!=-1 )
    {
		if(cg.snap->ps.clientNum == pug_demospec.integer){
			const char *configstring;
			configstring = CG_ConfigString( pug_demospec.integer + CS_PLAYERS );
			strcpy( cgs.clientinfo[ cg.snap->ps.clientNum ].name, Info_ValueForKey(configstring, "n") );
			pug_demospec.integer=-1;
		}
	}*/
	
    if( cg.demoPlayback && pug_demospec.integer!=-1 )
    {
		const char *configstring;
		int i;
		snap = *cg.snap;
		prps = cg.predictedPlayerState;
		//playerState_t ps;
		//int size;
		//size = &ps.generic1 - ps.stats;
        //cg.predictedPlayerState.clientNum = pug_demospec.integer;
        //memcpy(cg.snap->ps.stats,cg_entities[pug_demospec.integer].playerState->stats,size);
        //cg.snap->ps.clientNum = pug_demospec.integer;
        cg.snap->ps.fd = cg_entities[pug_demospec.integer].playerState->fd;
        cg.snap->ps.stats[STAT_DEAD_YAW]=cg_entities[pug_demospec.integer].playerState->stats[STAT_DEAD_YAW];
        for( i = 0; i < cgs.maxclients; i++ ) {
		    configstring = CG_ConfigString( i + CS_PLAYERS );
		    if( cgs.clientinfo[ i ].name[ strlen( cgs.clientinfo[ i ].name ) + 1 ] != -1 )
				strcpy( cgs.clientinfo[ i ].name, Info_ValueForKey(configstring, "n") );
		}
		if( cg.snap->ps.persistant[PERS_TEAM] == cgs.clientinfo[pug_demospec.integer].team && numSortedTeamPlayers ) {
			//only know health if its a teammate
			cg.snap->ps.stats[STAT_HEALTH] = cgs.clientinfo[pug_demospec.integer].health;
			cg.snap->ps.stats[STAT_ARMOR] = cgs.clientinfo[pug_demospec.integer].armor;
		} else {
			cg.snap->ps.stats[STAT_HEALTH] = cgs.clientinfo[pug_demospec.integer].health = cg_entities[pug_demospec.integer].currentState.eFlags & EF_DEAD?0:cgs.clientinfo[pug_demospec.integer].healthhax;
		}
		if( pug_demos_cameraMode.integer != 0 ) // plz 2 not interfere with special cams
			cg.snap->ps.stats[STAT_HEALTH] = cg.snap->ps.stats[STAT_MAX_HEALTH];
		if( cg_entities[pug_demospec.integer].currentState.eType != ET_PLAYER )
			cg.snap->ps.stats[STAT_HEALTH] = 1;
		cg.snap->ps.persistant[PERS_TEAM] = cgs.clientinfo[pug_demospec.integer].team;
		/*memcpy( cg.snap->ps.powerups, cg_entities[pug_demospec.integer].playerState->powerups, sizeof(int) * MAX_POWERUPS );
		memcpy( mypw, cg.predictedPlayerState.powerups, sizeof(int) * MAX_POWERUPS );
		memcpy( cg.predictedPlayerState.powerups, cg_entities[pug_demospec.integer].playerState->powerups, sizeof(int) * MAX_POWERUPS );*/
		orgState = cg.predictedPlayerState.pm_type;
		if(orgState == PM_DEAD)orgState = PM_NORMAL;
		cg.predictedPlayerState.pm_type = PM_NORMAL;
		if(cg_entities[pug_demospec.integer].currentState.eFlags & EF_DEAD) {
			cg.snap->ps.stats[STAT_DEAD_YAW] = cg_entities[pug_demospec.integer].lerpAngles[YAW];
			cg.predictedPlayerState.pm_type = PM_DEAD;
		}
		//messes up if original demo taker was free flight mode
		cg.snap->ps.fallingToDeath = cg_entities[pug_demospec.integer].playerState->fallingToDeath;
		cg.snap->ps.zoomMode = cg.predictedPlayerState.zoomMode = pug_demos_cameraMode.integer>0?0:cg_entities[pug_demospec.integer].playerState->zoomMode;
		cg.snap->ps.persistant[PERS_TEAM] = cgs.clientinfo[pug_demospec.integer].team;
		cg.snap->ps.viewheight = DEFAULT_VIEWHEIGHT;
		cg.predictedPlayerState.viewheight = DEFAULT_VIEWHEIGHT;
		cg.predictedPlayerState.weapon = cg_entities[pug_demospec.integer].currentState.weapon;
		cg.predictedPlayerState.weaponstate = cg_entities[pug_demospec.integer].currentState.modelindex2;
		
        //memcpy(&(cg.nextSnap->ps),cg_entities[pug_demospec.integer].playerState,sizeof(playerState_t));
        //ps = cg_entities[pug_demospec.integer].playerState;
        //CG_Printf("%d %d %d    %d %d %d\n",ps->origin[1],ps->origin[2],ps->origin[3],ps->viewangles[1],ps->viewangles[2],ps->viewangles[3]);
    }

	// decide on third person view
	cg.renderingThirdPerson = cg_thirdPerson.integer || (cg.snap->ps.stats[STAT_HEALTH] <= 0);

	if (cg.snap->ps.stats[STAT_HEALTH] > 0)
	{
		if (cg.predictedPlayerState.weapon == WP_EMPLACED_GUN && cg.predictedPlayerState.emplacedIndex /*&&
			cg_entities[cg.predictedPlayerState.emplacedIndex].currentState.weapon == WP_NONE*/)
		{ //force third person for e-web and emplaced use
			cg.renderingThirdPerson = 1;
		}
		else if (cg.predictedPlayerState.weapon == WP_SABER || cg.predictedPlayerState.weapon == WP_MELEE ||
			BG_InGrappleMove(cg.predictedPlayerState.torsoAnim) || BG_InGrappleMove(cg.predictedPlayerState.legsAnim) ||
			cg.predictedPlayerState.forceHandExtend == HANDEXTEND_KNOCKDOWN || cg.predictedPlayerState.fallingToDeath ||
			cg.predictedPlayerState.m_iVehicleNum || PM_InKnockDown(&cg.predictedPlayerState))
		{
			if (cg_fpls.integer && cg.predictedPlayerState.weapon == WP_SABER)
			{ //force to first person for fpls
				cg.renderingThirdPerson = 0;
			}
			else
			{
				cg.renderingThirdPerson = 1;
			}
		}
		else if (cg.snap->ps.zoomMode)
		{ //always force first person when zoomed
			cg.renderingThirdPerson = 0;
		}
	}
	
	if (cg.predictedPlayerState.pm_type == PM_SPECTATOR)
	{ //always first person for spec
		cg.renderingThirdPerson = 0;
	}


	if (cg.snap->ps.persistant[PERS_TEAM] == TEAM_SPECTATOR)
	{
		cg.renderingThirdPerson = 0;
	}
	
	if (demoCamming) cg.renderingThirdPerson = 1;
	
	if (pug_demos_cameraMode.integer == 2) cg.renderingThirdPerson = 0;
	if (pug_demos_cameraMode.integer == 3 || pug_demos_cameraMode.integer == 1) cg.renderingThirdPerson = 1;
	if (pug_demos_autoChaseCam.integer ) cg.renderingThirdPerson = 1;

	// build cg.refdef
	if( ( !(cg.demoPlayback && pug_demospec.integer!=-1 ) && !(/*cg.demoPlayback && */demoCamming==qtrue) ) || 
		( pug_demos_cameraMode.integer == 1 || ( (pug_demos_cameraMode.integer == 2 || pug_demos_cameraMode.integer == 3) && pug_demospec.integer == -1 ) ) ) {
		if( cg.demoPlayback && ( pug_demos_cameraMode.integer == 1 || pug_demos_cameraMode.integer == 2 ) ) {
			int oldTP;
			oldTP = cg.renderingThirdPerson;
			cg.renderingThirdPerson = 0;
			swapPlayerstates( );
			inwater = CG_CalcViewValues();
			swapPlayerstates( );
			cg.renderingThirdPerson = oldTP;
		} else if( cg.demoPlayback && pug_demos_cameraMode.integer == 3 ) {
			int oldTP;
			oldTP = cg.renderingThirdPerson;
			cg.renderingThirdPerson = 1;
			swapPlayerstates( );
			inwater = CG_CalcViewValues();
			swapPlayerstates( );
			cg.renderingThirdPerson = oldTP;
			VectorCopy( cg.truePredictedPlayerState.viewangles, cg.refdef.viewangles );
	        AnglesToAxis( cg.refdef.viewangles, cg.refdef.viewaxis );
		} else
			inwater = CG_CalcViewValues();
	}
	if (cg_linearFogOverride)
	{
		trap_R_SetRangeFog(-cg_linearFogOverride);
	}
	else if (cg.predictedPlayerState.zoomMode)
	{ //zooming with binoculars or sniper, set the fog range based on the zoom level -rww
		cg_rangedFogging = qtrue;
		//smaller the fov the less fog we have between the view and cull dist
		trap_R_SetRangeFog(cg.refdef.fov_x*64.0f);
	}
	else if (cg_rangedFogging)
	{ //disable it
		cg_rangedFogging = qfalse;
		trap_R_SetRangeFog(0.0f);
	}

	cstr = CG_ConfigString(CS_SKYBOXORG);

	if (cstr && cstr[0])
	{ //we have a skyportal
		CG_DrawSkyBoxPortal(cstr);
	}

	CG_CalcScreenEffects();

	// first person blend blobs, done after AnglesToAxis
	if ( !cg.renderingThirdPerson && cg.predictedPlayerState.pm_type != PM_SPECTATOR ) {
		CG_DamageBlendBlob();
	}
	
	if( cg.demoPlayback && pug_demospec.integer!=-1 )
    {
		centity_t *cent;
		int orgFlags;
        cent = cg_entities + pug_demospec.integer;
        cg.predictedPlayerState.weapon = prps.weapon;
        cg.predictedPlayerState.weaponstate = prps.weaponstate;
        if( demoCamming == qfalse && pug_demos_cameraMode.integer != 1 ) {
			VectorCopy( cent->lerpOrigin, cg.refdef.vieworg );
	        VectorCopy( cent->lerpAngles, cg.refdef.viewangles );
	        AnglesToAxis( cg.refdef.viewangles, cg.refdef.viewaxis );
		}
		if( demoCamming == qfalse && ( pug_demos_cameraMode.integer == 2 || pug_demos_cameraMode.integer == 3 ) ) {
			VectorCopy( cg.truePredictedPlayerState.viewangles, cg.refdef.viewangles );
	        AnglesToAxis( cg.refdef.viewangles, cg.refdef.viewaxis );
		}
        cg.snap->ps.m_iVehicleNum = cg_entities[pug_demospec.integer].playerState->m_iVehicleNum;
        cg.predictedPlayerState.pm_type = orgState;
        orgFlags = cg.predictedPlayerState.pm_flags;
        cg.predictedPlayerState.pm_flags = 0;
        if( demoCamming == qfalse && pug_demos_cameraMode.integer != 1 ) {
	        if(cg.renderingThirdPerson && pug_demos_cameraMode.integer != 1)CG_OffsetThirdPersonView();
	        else CG_OffsetFirstPersonView();
	        CG_CalcFov();
		}
        cg.predictedPlayerState.pm_flags = orgFlags;
        /*memcpy( cg.snap->ps.powerups, snap.ps.powerups, sizeof(int) * MAX_POWERUPS );
        memcpy( cg.predictedPlayerState.powerups, mypw, sizeof(int) * MAX_POWERUPS );*/
    }
    
	if( ghostFile ) {
		switch( ghostMode ) {
			case qtrue:
			{
				static vec3_t vieworg,viewangles/*,viewaxis[3]*//*,nextViewaxis[3]*/;
				static int servTime=0, nextServTime=0;
				static FILE *lastFile;
				int i, myTime;
				double timeDelta;
				if( ghostFile != lastFile ) 
				{
					servTime = 0;
					nextServTime = 0;
					lastFile = ghostFile;
				}
				myTime = ( camPreviewTime ? (cg.time - camPreviewTime) + cam[0].time: cg.time - cgs.levelStartTime );
				if( myTime < servTime )
				{
					// rewind ghost file brah
					//fseek( ghostFile, 0, SEEK_SET );
					rewind( ghostFile );
					servTime = nextServTime = 0;
				}
				while( myTime > nextServTime ) {
					//push next down to current, read next from file
					VectorCopy( nextVieworg, vieworg );
					VectorCopy( nextViewangles, viewangles );
					//VectorCopy( nextViewaxis[0], viewaxis[0] );
					//VectorCopy( nextViewaxis[1], viewaxis[1] );
					//VectorCopy( nextViewaxis[2], viewaxis[2] );
					servTime = nextServTime;
					fread( &nextVieworg, sizeof( vec3_t ), 1, ghostFile );
					fread( &nextViewangles, sizeof( vec3_t ), 1, ghostFile );
					//fread( &nextViewaxis, sizeof( vec3_t ) * 3, 1, ghostFile );
					fread( &nextServTime, sizeof( int ), 1, ghostFile );
					if( nextServTime <= servTime ) {
						if( demoPlayback ) {
							fclose( ghostFile );
							ghostFile = 0;
							CG_Printf( "End of ghost reached.\n" );
						} else {
							rewind( ghostFile );
							servTime = nextServTime = 0;
						}
						break;
					}
				}
				if( servTime != 0 )
				{
					vec3_t atOrg, dir2Me;
					// fix potential angle wrapping interpolation issues
					// by minimizing the difference
					for( i = 0; i < 3; i++ )
					{
						if( viewangles[i] - nextViewangles[i] > 180 ) {
							viewangles[i] -= 360;
						} else if( viewangles[i] - nextViewangles[i] < -180 ) {
							viewangles[i] += 360;
						}
					}
					//interpolate current cam pos from the 2 we have stored
					timeDelta = (double)( nextServTime - myTime ) / (double)( nextServTime - servTime );
					if( !cg.demoPlayback )
					{
						refEntity_t ent;
						vec3_t ang;
						float scale=0.5f;
						vec3_t viewDir;
						
						memset( &ent, 0, sizeof( ent ) );
						
						for( i=0; i<3; i++ ) {
							ent.origin[i] = vieworg[i] * timeDelta + nextVieworg[i] * (1 - timeDelta);
							//ent.[i] = viewangles[i] * timeDelta + nextViewangles[i] * (1 - timeDelta);
						}
						
						VectorSubtract(ent.origin, cg.refdef.vieworg, ent.axis[0]);
						
						VectorCopy(ent.axis[0], viewDir);
						VectorInverse(viewDir);
						VectorNormalize(viewDir);
						
						vectoangles(ent.axis[0], ang);
						ang[ROLL] += 180.0f;
						ang[PITCH] += 180.0f;
						AnglesToAxis(ang, ent.axis);
						
						VectorScale(ent.axis[0], scale, ent.axis[0]);
						VectorScale(ent.axis[1], scale, ent.axis[1]);
						VectorScale(ent.axis[2], scale, ent.axis[2]);
						
						ent.nonNormalizedAxes = qtrue;
						
						ent.hModel = cgs.media.halfShieldModel;
						ent.customShader = cgs.media.invulnerabilityShader;
						
						ent.shaderRGBA[0] = 0;
						ent.shaderRGBA[1] = 0;
						ent.shaderRGBA[2] = 255;
						ent.shaderRGBA[3] = 100;
						ent.renderfx = RF_RGB_TINT;

						if(trap_R_inPVS(cg.refdef.vieworg, ent.origin, cg.snap->areamask))trap_R_AddRefEntityToScene( &ent );
						
						if( demoCamming && pug_demos_cameraMode.integer>4 ) {
							vec3_t curVeca;
							vec3_t dir2Me;
							//point at current focused ghost
							VectorSubtract( ent.origin, cg.refdef.vieworg, dir2Me );
							vectoangles( dir2Me, curVeca );
							curVeca[2] = cg.refdef.viewangles[2];
							if(pug_demos_cameraMode.integer == 6 )
								curVeca[PITCH] = cg.refdef.viewangles[PITCH];
							if(pug_demos_cameraMode.integer == 7 )
								curVeca[YAW] = cg.refdef.viewangles[YAW];
							VectorCopy( curVeca, cg.refdef.viewangles );
							AnglesToAxis( cg.refdef.viewangles, cg.refdef.viewaxis );
					        CG_CalcFov();
						}
					} else {
						for( i=0; i<3; i++ ) {
							cg.refdef.vieworg[i] = vieworg[i] * timeDelta + nextVieworg[i] * (1 - timeDelta);
							cg.refdef.viewangles[i] = viewangles[i] * timeDelta + nextViewangles[i] * (1 - timeDelta);
							//cg.refdef.viewaxis[0][i] = viewaxis[0][i] * timeDelta + nextViewaxis[0][i] * (1 - timeDelta);
							//cg.refdef.viewaxis[1][i] = viewaxis[1][i] * timeDelta + nextViewaxis[1][i] * (1 - timeDelta);
							//cg.refdef.viewaxis[2][i] = viewaxis[2][i] * timeDelta + nextViewaxis[2][i] * (1 - timeDelta);
						}
						if(pug_demos_cameraMode.integer>4) {
							//point at current focused player
							VectorCopy( cg_entities[pug_demospec.integer!=-1?pug_demospec.integer:cg.predictedPlayerState.clientNum].lerpOrigin, atOrg );
							VectorSubtract( atOrg, cg.refdef.vieworg, dir2Me );
							vectoangles( dir2Me, cg.refdef.viewangles );
							cg.refdef.viewangles[2] = viewangles[2] * timeDelta + nextViewangles[2] * (1 - timeDelta);
						}
						if(pug_demos_cameraMode.integer == 6 )
							cg.refdef.viewangles[PITCH] = viewangles[0] * timeDelta + nextViewangles[0] * (1 - timeDelta);
						if(pug_demos_cameraMode.integer == 7 )
							cg.refdef.viewangles[YAW] = viewangles[1] * timeDelta + nextViewangles[1] * (1 - timeDelta);
						AnglesToAxis( cg.refdef.viewangles, cg.refdef.viewaxis );
					}
				}
				break;
			}
			case qfalse:
			{
				int tmp;
				tmp = serverTime - cgs.levelStartTime;
				fwrite( cg.refdef.vieworg, sizeof( vec3_t ), 1, ghostFile );
				fwrite( cg.refdef.viewangles, sizeof( vec3_t ), 1, ghostFile );
				//trap_FS_Write( cg.refdef.viewaxis, sizeof( vec3_t ) * 3, ghostFile );
				fwrite( &tmp, sizeof( int ), 1, ghostFile );
				break;
			}
		}
	}
    
	// build the render lists
	if ( !cg.hyperspace ) {
		CG_AddPacketEntities(qfalse);			// adter calcViewValues, so predicted player state is correct
		CG_AddMarks();
		CG_AddParticles ();
		CG_AddLocalEntities();
		CG_DrawMiscEnts();
	}
	if( cg.demoPlayback && pug_demospec.integer!=-1 )
	{
		int orgNum;
		orgNum = cg.predictedPlayerState.clientNum;
		cg.predictedPlayerState.clientNum = pug_demospec.integer;
		CG_AddViewWeapon( &cg.predictedPlayerState );
		cg.predictedPlayerState.clientNum = orgNum;
		if(cg_entities[pug_demospec.integer].currentState.eFlags & EF_DEAD)cg.predictedPlayerState.pm_type = PM_DEAD;
	} else
		CG_AddViewWeapon( &cg.predictedPlayerState );
	
	if ( !cg.hyperspace) 
	{
		trap_FX_AddScheduledEffects(qfalse);
	}

	// add buffered sounds
	CG_PlayBufferedSounds();

	// finish up the rest of the refdef
	if ( cg.testModelEntity.hModel ) {
		CG_AddTestModel();
	}
	cg.refdef.time = cg.time;
	memcpy( cg.refdef.areamask, cg.snap->areamask, sizeof( cg.refdef.areamask ) );

	// warning sounds when powerup is wearing off
	CG_PowerupTimerSounds();

	// if there are any entities flagged as sound trackers and attached to other entities, update their sound pos
	CG_UpdateSoundTrackers();

	if (gCGHasFallVector && pug_demos_cameraMode.integer == 0 && !(ghostFile && ghostMode))
	{
		vec3_t lookAng;

		VectorSubtract(cg.snap->ps.origin, cg.refdef.vieworg, lookAng);
		VectorNormalize(lookAng);
		vectoangles(lookAng, lookAng);

		VectorCopy(gCGFallVector, cg.refdef.vieworg);
		AnglesToAxis(lookAng, cg.refdef.viewaxis);
		//CG_Printf("fallin");
	}

	//This is done from the vieworg to get origin for non-attenuated sounds
	cstr = CG_ConfigString( CS_GLOBAL_AMBIENT_SET );

	if (cstr && cstr[0])
	{
		trap_S_UpdateAmbientSet( cstr, cg.refdef.vieworg );
	}

	// update audio positions
	trap_S_Respatialize( pug_demospec.integer!=-1?pug_demospec.integer:cg.snap->ps.clientNum, cg.refdef.vieworg, cg.refdef.viewaxis, inwater );

	// make sure the lagometerSample and frame timing isn't done twice when in stereo
	if ( stereoView != STEREO_RIGHT ) {
		cg.frametime = cg.time - cg.oldTime;
		if ( cg.frametime < 0 ) {
			cg.frametime = 0;
		}
		cg.oldTime = cg.time;
		CG_AddLagometerFrameInfo();
	}
	if (cg_timescale.value != cg_timescaleFadeEnd.value) {
		if (cg_timescale.value < cg_timescaleFadeEnd.value) {
			cg_timescale.value += cg_timescaleFadeSpeed.value * ((float)cg.frametime) / 1000;
			if (cg_timescale.value > cg_timescaleFadeEnd.value)
				cg_timescale.value = cg_timescaleFadeEnd.value;
		}
		else {
			cg_timescale.value -= cg_timescaleFadeSpeed.value * ((float)cg.frametime) / 1000;
			if (cg_timescale.value < cg_timescaleFadeEnd.value)
				cg_timescale.value = cg_timescaleFadeEnd.value;
		}
		if (cg_timescaleFadeSpeed.value) {
			trap_Cvar_Set("timescale", va("%f", cg_timescale.value));
		}
	}
	
	if( pug_demos_autoChaseCam.integer )
	{
		// automatically render third-person views using a "lazy cam" approach V :S V
		static vec3_t camOrigin = { 0, 0, 0 }, camAngles = { 0, 0, 0 }, camVelocity = { 0, 0, 0 };
		static int lastTime;
		int msec;
		vec3_t tmpVec, curOrigin;
		float distThreshold = 60.0f, accelMax = 10.0f, velMax = 1500, tmpFloat;
		VectorCopy( cg_entities[pug_demospec.integer!=-1?pug_demospec.integer:cg.predictedPlayerState.clientNum].lerpOrigin, curOrigin );
		curOrigin[2] += DEFAULT_VIEWHEIGHT;
		msec = serverTime - lastTime;
		lastTime = serverTime;
		VectorSubtract( curOrigin, camOrigin, tmpVec );
		if( msec > 5000 || VectorLength( tmpVec ) > 5000 )
		{
			VectorCopy( /*cg.refdef.vieworg*/curOrigin, camOrigin );
			VectorSet( camVelocity, 0, 0, 0 );
		}
		VectorSubtract( curOrigin, camOrigin, tmpVec );
		tmpFloat = VectorLength( tmpVec );
		//CG_Printf("%f ",tmpFloat);
		if( tmpFloat )
		{
			VectorScale( tmpVec, ( tmpFloat - distThreshold ) / tmpFloat, tmpVec );
		}
		tmpFloat = VectorLength( tmpVec );
		//CG_Printf("%f | ",tmpFloat);
		// scale by half
		//VectorScale( tmpVec, 0.5f, tmpVec );
		// tmpVec now holds the distance from the current cam origin to the closest point on the player sphere
		VectorSubtract( tmpVec, camVelocity, tmpVec );
		// the difference is our acceleration
		// ideally we would add tmpVec to camVelocity but it'd be pretty unrealistic so we cap it
		/*if( VectorLength( tmpVec ) > accelMax )
		{
			CG_Printf("%f\n", VectorLength( tmpVec ) );
			VectorNormalize( tmpVec );
			VectorScale( tmpVec, accelMax, tmpVec );
		}*/
		// velocity is in units / second i guess =/
		// so accel is in units / second^2
		VectorScale( tmpVec, (float)msec / 1000, tmpVec );
		// now we should have a vector to add to our velocity vector in order to better adjust our cam pos
		VectorAdd( tmpVec, camVelocity, camVelocity );
		
		// velocity is in units / second i guess =/
		/*if( VectorLength( camVelocity ) > velMax )
		{
			VectorNormalize( camVelocity );
			VectorScale( camVelocity, velMax, camVelocity );
		}*/
		VectorScale( camVelocity, (float)msec / 1000, tmpVec );
		//CG_Printf("%f %f %f | ", tmpVec[0], tmpVec[1], tmpVec[2] );
		VectorAdd( camOrigin, tmpVec, camOrigin );
		/*camOrigin[0] += camVelocity[0] * msec / 1000;
		camOrigin[1] += camVelocity[1] * msec / 1000;
		camOrigin[2] += camVelocity[2] * msec / 1000;*/
		// now just look at player
		VectorSubtract( curOrigin, camOrigin, tmpVec );
		vectoangles( tmpVec, camAngles );
		//VectorNormalize( camAngles );
		// copy to refdef
		VectorCopy( camOrigin, cg.refdef.vieworg );
		VectorCopy( camAngles, cg.refdef.viewangles );
		AnglesToAxis( cg.refdef.viewangles, cg.refdef.viewaxis );
		//CG_Printf("%f %f %f | %f %f %f\n", camOrigin[0], camOrigin[1], camOrigin[2],
		//	camVelocity[0], camVelocity[1], camVelocity[2] );
	}
	
	if ( !cg.demoPlayback ) drawSplines( );
	
	/*if( CG_Cvar_Int( "cl_freezedemo" ) == 1 )
	{
		static qboolean dun=qfalse;
		extern void reCalc( );
		if( !dun )
		{
			int insertCam, j;
			dun = !dun;
			
			insertCam = 0;
			//push cams >= down one
			for ( j=curCam-1; j>=insertCam; j-- )
			{
				cam[ j+1 ] = cam[ j ];
			}
			//origins
			VectorCopy( cg.refdef.vieworg, cam[insertCam].origin );
			//angles
			VectorCopy( cg.refdef.viewangles, cam[insertCam].angle );
			cam[insertCam].angle[0] += 360;
			cam[insertCam].time = serverTime - cgs.levelStartTime;
			curCam++;
			
			//origins
			VectorCopy( cg.refdef.vieworg, cam[curCam].origin );
			//angles
			VectorCopy( cg.refdef.viewangles, cam[curCam].angle );
			cam[curCam].angle[1] -= 360;
			cam[curCam].angle[0] += 360;
			cam[curCam].time = ( ( 8 ) * 60000 + ( 52 ) * 1000 );
			reCalc();
			curCam++;
		}
	}*/

	// actually issue the rendering calls
	CG_DrawActive( stereoView );

	CG_DrawAutoMap();

	if ( cg_stats.integer ) {
		CG_Printf( "cg.clientFrame:%i\n", cg.clientFrame );
	}
	
    if( cg.demoPlayback && pug_demospec.integer!=-1 )
    {
        //cg.snap->ps.clientNum = snap.ps.clientNum;
        cg.snap->ps.fd = snap.ps.fd;
        cg.snap->ps.stats[STAT_DEAD_YAW] = snap.ps.stats[STAT_DEAD_YAW];
		cg.snap->ps.stats[STAT_HEALTH] = snap.ps.stats[STAT_HEALTH];
		cg.snap->ps.stats[STAT_ARMOR] = snap.ps.stats[STAT_ARMOR];
		cg.snap->ps.persistant[PERS_TEAM] = snap.ps.persistant[PERS_TEAM];
		cg.snap->ps.fallingToDeath = snap.ps.fallingToDeath;
		cg.snap->ps.weapon = snap.ps.weapon;
		cg.snap->ps.weaponstate = snap.ps.weaponstate;
		/*memcpy( cg.snap->ps.powerups, snap.ps.powerups, sizeof(int) * MAX_POWERUPS );*/
		cg.snap->ps.zoomMode = cg.predictedPlayerState.zoomMode = snap.ps.zoomMode;
		cg.predictedPlayerState = prps;
		//cg.snap->ps = snap.ps;
    }
    if( cg.demoPlayback && ( pug_demos_cameraMode.integer == 1 || pug_demos_cameraMode.integer == 2 || pug_demos_cameraMode.integer == 3 ) ) swapPlayerstates( );
}

extern int showCam;
void drawSplines( void )
{
	int 		myTime, i;
	qboolean 	demoCamming = qtrue;
	polyVert_t	verts[3];
	double		l_time;
	
	myTime = cam[0].time;
	if ( myTime == 0 ) return;
	
	while ( demoCamming )
	{
		demoCamming = qfalse;
		for(i = 0;i < curCam - 1;i++) {
			if(cam[i].time!=0 &&
			   cam[i].time <= myTime &&
			   cam[i+1].time > myTime &&
			   cam[i+1].time > cam[i].time &&
			   !cam[i].thirdPerson &&
			   !cam[i+1].thirdPerson)
			{
				demoCamming = qtrue;
				break;
			}
		}
		if ( demoCamming )
		{
			//timeDelta = (double)(myTime - cam[i].time) / (double)(cam[i+1].time - cam[i].time);
			l_time=((double)(myTime - cam[i].time))/1000;
			
			VectorCopy( verts[2].xyz, verts[0].xyz );
			
			verts[0].st[0] = 0;
			verts[0].st[1] = 0;
			
			verts[1].st[0] = 0;
			verts[1].st[1] = 1;
			
			verts[0].modulate[0] = verts[1].modulate[0] = 255;
			verts[0].modulate[1] = verts[1].modulate[1] = 255;
			verts[0].modulate[2] = verts[1].modulate[2] = 255;
			verts[0].modulate[3] = verts[1].modulate[3] = 255;
			
//			verts[2].xyz[0] = cam[i].csi_coeff[0][0]*l_time*l_time*l_time+cam[i].csi_coeff[0][1]*l_time*l_time+cam[i].csi_coeff[0][2]*l_time+cam[i].csi_coeff[0][3];
//			verts[2].xyz[1] = cam[i].csi_coeff[1][0]*l_time*l_time*l_time+cam[i].csi_coeff[1][1]*l_time*l_time+cam[i].csi_coeff[1][2]*l_time+cam[i].csi_coeff[1][3];
//			verts[2].xyz[2] = cam[i].csi_coeff[2][0]*l_time*l_time*l_time+cam[i].csi_coeff[2][1]*l_time*l_time+cam[i].csi_coeff[2][2]*l_time+cam[i].csi_coeff[2][3];
			{
				double time = (myTime - cam[0].time) / (double) (cam[curCam - 1].time - cam[0].time);
				verts[2].xyz[0] = computePosition(0, time);
				verts[2].xyz[1] = computePosition(1, time);
				verts[2].xyz[2] = computePosition(2, time);
				// CG_Printf("Time: %lf, NumCams=%d, Position = {x=%lf,y=%lf,z=%lf}\n", time, curCam, verts[2].xyz[0], verts[2].xyz[1], verts[2].xyz[2]);
			}
			// VectorAdd( verts[2].xyz, cam[i].origin, verts[2].xyz );
			VectorCopy( verts[2].xyz, verts[1].xyz );
			
			//verts[1].xyz[0] += 3.0f;
			
			verts[2].st[0] = 1;
			verts[2].st[1] = 1;
			
			verts[2].modulate[0] = 255;
			verts[2].modulate[1] = 255;
			verts[2].modulate[2] = 255;
			verts[2].modulate[3] = 255;
			
			/*curVeca[0] = cam[i].csi_coeff[3][0]*l_time*l_time*l_time+cam[i].csi_coeff[3][1]*l_time*l_time+cam[i].csi_coeff[3][2]*l_time+cam[i].csi_coeff[3][3];
			curVeca[1] = cam[i].csi_coeff[4][0]*l_time*l_time*l_time+cam[i].csi_coeff[4][1]*l_time*l_time+cam[i].csi_coeff[4][2]*l_time+cam[i].csi_coeff[4][3];
			curVeca[2] = cam[i].csi_coeff[5][0]*l_time*l_time*l_time+cam[i].csi_coeff[5][1]*l_time*l_time+cam[i].csi_coeff[5][2]*l_time+cam[i].csi_coeff[5][3];
			VectorAdd( curVeca, cam[i].angle, curVeca );*/
			
			if ( myTime != cam[0].time && trap_R_inPVS(cg.refdef.vieworg, verts[2].xyz, cg.snap->areamask)) {
				/*float fac, sine;
				vec3_t tmpvec;
				
				VectorSubtract( verts[2].xyz, verts[0].xyz, verts[1].xyz );
				fac = VectorLength( verts[1].xyz );
				verts[1].xyz[0] /= fac;
				verts[1].xyz[1] /= fac;
				verts[1].xyz[2] /= fac;
				sine = asin( verts[1].xyz[1] );
				if( sine < M_PI / 2 )
				{
					if( verts[1].xyz[1] > 0 && verts[1].xyz[0] < 0 ) sine = M_PI - sine;
				}
				else if( sine > M_PI && sine < M_PI * 3 / 2 )
				{
					if( verts[1].xyz[1] < 0 && verts[1].xyz[0] > 0 ) sine = ( 3 * M_PI ) - sine;
				}
				sine += ( 5.0f / 180.0f * M_PI ); //add 5 degrees
				verts[1].xyz[1] = sin( sine ); //convert back 2 coordinate
				verts[1].xyz[0] *= fac;
				verts[1].xyz[1] *= fac;
				verts[1].xyz[2] *= fac;
				VectorAdd( verts[0].xyz, verts[1].xyz, verts[1].xyz );
				VectorCopy( verts[1].xyz, tmpvec );
				
				trap_R_AddPolyToScene( 2, 3, verts );
				
				VectorSubtract( verts[2].xyz, verts[0].xyz, verts[1].xyz );
				fac = VectorLength( verts[1].xyz );
				verts[1].xyz[0] /= fac;
				verts[1].xyz[1] /= fac;
				verts[1].xyz[2] /= fac;
				sine = asin( verts[1].xyz[2] );
				if( sine < M_PI / 2 )
				{
					if( verts[1].xyz[2] > 0 && verts[1].xyz[0] < 0 ) sine = M_PI - sine;
				}
				else if( sine > M_PI && sine < M_PI * 3 / 2 )
				{
					if( verts[1].xyz[2] < 0 && verts[1].xyz[0] > 0 ) sine = ( 3 * M_PI ) - sine;
				}
				sine += ( 5.0f / 180.0f * M_PI ); //add 5 degrees
				verts[1].xyz[2] = sin( sine ); //convert back 2 coordinate
				verts[1].xyz[0] *= fac;
				verts[1].xyz[1] *= fac;
				verts[1].xyz[2] *= fac;
				VectorAdd( verts[0].xyz, verts[1].xyz, verts[1].xyz );
				
				trap_R_AddPolyToScene( 2, 3, verts );
				
				VectorCopy( tmpvec, verts[2].xyz );
				
				trap_R_AddPolyToScene( 2, 3, verts );*/
				//CG_TestLine( verts[0].xyz, verts[2].xyz, 2, 0, 1 );
				#include "fx_local.h"
				static vec3_t WHITE={1.0f,1.0f,1.0f};
				trap_FX_AddLine( verts[0].xyz, verts[2].xyz, 0.1f, 6.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							150, trap_R_RegisterShader( "gfx/effects/redLine" ), 
							FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
			
			
		}
		
		myTime += (double)( cam[ i+1 ].time - cam[ i ].time ) / (double)20;
		if( myTime >= cam[ i+1 ].time && cam[ i+2 ].time < cam[ i+1 ].time ) {
			VectorCopy( verts[2].xyz, verts[0].xyz );
			VectorCopy( cam[ i+1 ].origin, verts[2].xyz );
			VectorCopy( verts[2].xyz, verts[1].xyz );
			verts[1].xyz[0] += 3.0f;
			if(trap_R_inPVS(cg.refdef.vieworg, verts[2].xyz, cg.snap->areamask)) {
				static vec3_t WHITE={1.0f,1.0f,1.0f};
				trap_FX_AddLine( verts[0].xyz, verts[2].xyz, 0.1f, 6.0f, 0.0f, 
							1.0f, 0.0f, 0.0f,
							WHITE, WHITE, 0.0f,
							150, trap_R_RegisterShader( "gfx/effects/redLine" ), 
							FX_SIZE_LINEAR | FX_ALPHA_LINEAR );
			}
		}
	}
	
	for ( i=0; cam[i].time; i++ )
	{
		//CG_DrawPlayerSphere(cent, cent->lerpOrigin, 1.0f, cgs.media.invulnerabilityShader );
		refEntity_t ent;
		vec3_t ang;
		float vLen, scale=0.5f;
		vec3_t viewDir;
		
		memset( &ent, 0, sizeof( ent ) );
		
		VectorCopy( cam[i].origin, ent.origin );
		//ent.origin[2] += 9.0;
		
		VectorSubtract(ent.origin, cg.refdef.vieworg, ent.axis[0]);
		vLen = VectorLength(ent.axis[0]);
		if (vLen <= 0.1f)
		{	// Entity is right on vieworg.  quit.
			continue;
		}
		
		VectorCopy(ent.axis[0], viewDir);
		VectorInverse(viewDir);
		VectorNormalize(viewDir);
		
		vectoangles(ent.axis[0], ang);
		ang[ROLL] += 180.0f;
		ang[PITCH] += 180.0f;
		AnglesToAxis(ang, ent.axis);
		
		VectorScale(ent.axis[0], scale, ent.axis[0]);
		VectorScale(ent.axis[1], scale, ent.axis[1]);
		VectorScale(ent.axis[2], scale, ent.axis[2]);
		
		ent.nonNormalizedAxes = qtrue;
		
		ent.hModel = cgs.media.halfShieldModel;
		ent.customShader = cgs.media.invulnerabilityShader;
		
		if( showCam == i )
		{
			ent.shaderRGBA[0] = 255;
			ent.shaderRGBA[1] = 255;
			ent.shaderRGBA[2] = 255;
			ent.shaderRGBA[3] = 100;
			ent.renderfx = RF_RGB_TINT;
		}
		
		if(trap_R_inPVS(cg.refdef.vieworg, ent.origin, cg.snap->areamask))trap_R_AddRefEntityToScene( &ent );
		
		/*ang[PITCH] -= 180.0f;
		AnglesToAxis(ang, ent.axis);
		
		VectorScale(ent.axis[0], scale*0.5f, ent.axis[0]);
		VectorScale(ent.axis[1], scale*0.5f, ent.axis[1]);
		VectorScale(ent.axis[2], scale*0.5f, ent.axis[2]);
		
		ent.renderfx = (RF_DISTORTION|RF_FORCE_ENT_ALPHA);
		ent.shaderRGBA[0] = 0;
		ent.shaderRGBA[1] = 255;
		ent.shaderRGBA[2] = 0;
		ent.shaderRGBA[3] = 100;
		ent.radius = 256;
		
		VectorMA(ent.origin, 40.0f, viewDir, ent.origin);
		
		ent.customShader = trap_R_RegisterShader("effects/refract_2");
		trap_R_AddRefEntityToScene( &ent );*/
	}
}
