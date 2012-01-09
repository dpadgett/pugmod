// Rocket Launcher Weapon

#include "cg_local.h"

/*
---------------------------
FX_RocketProjectileThink
---------------------------
*/

/*
==========================
CG_RocketTrail
==========================
*/
static void CG_RocketTrail( centity_t *ent, const weaponInfo_t *wi ) {
	int		step;
	vec3_t	origin, lastPos;
	int		t;
	int		startTime, contents;
	int		lastContents;
	entityState_t	*es;
	vec3_t	up;
	localEntity_t	*smoke, *firetail;
	refEntity_t		*flare;
	static int	seed = 0x92;

	if ( cg_noProjectileTrail.integer ) {
		return;
	}

	up[0] = 0;
	up[1] = 0;
	up[2] = 0;

	step = 10;

	es = &ent->currentState;
	startTime = ent->trailTime;
	t = step * ( (startTime + step) / step );

	BG_EvaluateTrajectory( &es->pos, cg.time, origin );
	contents = CG_PointContents( origin, -1 );

	// if object (e.g. grenade) is stationary, don't toss up smoke
	if ( es->pos.trType == TR_STATIONARY ) {
		ent->trailTime = cg.time;
		return;
	}

	BG_EvaluateTrajectory( &es->pos, ent->trailTime, lastPos );
	lastContents = CG_PointContents( lastPos, -1 );

	ent->trailTime = cg.time;

	if ( contents & ( CONTENTS_WATER | CONTENTS_SLIME | CONTENTS_LAVA ) ) {
		if ( contents & lastContents & CONTENTS_WATER ) {
			CG_BubbleTrail( lastPos, origin, 8 );
		}
		return;
	}

	for ( ; t <= ent->trailTime ; t += step ) {
		BG_EvaluateTrajectory( &es->pos, t, lastPos );

		smoke = CG_SmokePuff( lastPos, up, 
					  wi->trailRadius, 
					  1, 1, 1, 0.66f,
					  wi->wiTrailTime, 
					  t,
					  0,
					  0, 
					  cgs.media.smokePuffShader );
		// use the optimized local entity add
		smoke->data.rocketSmoke.life = irand( 300, 500 );
		smoke->endTime = smoke->startTime + smoke->data.rocketSmoke.life;
		smoke->data.rocketSmoke.rgb_s[0] = flrand( 0.7216, 0.8431 );
		smoke->data.rocketSmoke.rgb_s[1] = flrand( 0.7176, 0.8431 );
		smoke->data.rocketSmoke.rgb_s[2] = flrand( 0.5373, 0.7569 );
		smoke->data.rocketSmoke.rgb_e[0] = 0.2353;
		smoke->data.rocketSmoke.rgb_e[1] = 0.2353;
		smoke->data.rocketSmoke.rgb_e[2] = 0.2353;
		smoke->data.rocketSmoke.alpha_s = flrand( 0.7, 0.8 );
		smoke->data.rocketSmoke.alpha_e = 0.1;
		smoke->refEntity.renderfx = RF_RGB_TINT | RF_FORCE_ENT_ALPHA;
		smoke->data.rocketSmoke.size_s = flrand( 3, 4 );
		smoke->data.rocketSmoke.size_e = flrand( 6, 12 );
		smoke->leType = LE_ROCKET_SMOKE;
	}
	
	// ******* FIRE TAIL CODE ************
	
	step = 10;
	
	t = step * ( (startTime + step) / step );
	for ( ; t <= ent->trailTime ; t += step ) {
		BG_EvaluateTrajectory( &es->pos, t, lastPos );

		firetail = CG_AllocLocalEntity();
		firetail->leFlags = 0;
		firetail->radius = 0;
		firetail->leType = LE_ROCKET_TRAIL;
		firetail->startTime = t;
		firetail->data.rocketTrail.life = irand( 10, 20 );
		firetail->data.rocketTrail.rgb_s[0] = 1;
		firetail->data.rocketTrail.rgb_s[1] = flrand( 0, 0.502 );
		firetail->data.rocketTrail.rgb_s[2] = flrand( 0, 0.251 );
		VectorCopy( firetail->data.rocketTrail.rgb_s, firetail->data.rocketTrail.rgb_e );
		firetail->data.rocketTrail.alpha_s = flrand( 0.8, 0.9 );
		firetail->data.rocketTrail.alpha_e = 0;
		firetail->data.rocketTrail.size_s = firetail->data.rocketTrail.size_e = flrand( 5, 7 );
		firetail->data.rocketTrail.length_s = flrand( 65, 70 );
		firetail->data.rocketTrail.length_e = 60;
		firetail->data.rocketTrail.hshader = cgs.media.flamejetShader;
		firetail->endTime = firetail->startTime + firetail->data.rocketTrail.life;
		VectorCopy( lastPos, firetail->pos.trBase );
		VectorCopy( es->pos.trDelta, firetail->pos.trDelta );
	}
	
	firetail = CG_AllocLocalEntity();
	firetail->leFlags = 0;
	firetail->radius = 0;
	firetail->leType = LE_ROCKET_TRAIL;
	firetail->startTime = cg.time;
	firetail->data.rocketTrail.life = 1;
	firetail->data.rocketTrail.rgb_s[0] = 1;
	firetail->data.rocketTrail.rgb_s[1] = flrand( 0, 0.502 );
	firetail->data.rocketTrail.rgb_s[2] = flrand( 0, 0.251 );
	VectorCopy( firetail->data.rocketTrail.rgb_s, firetail->data.rocketTrail.rgb_e );
	firetail->data.rocketTrail.alpha_s = flrand( 0.8, 0.9 );
	firetail->data.rocketTrail.alpha_e = 0;
	firetail->data.rocketTrail.size_s = firetail->data.rocketTrail.size_e = flrand( 5, 7 );
	firetail->data.rocketTrail.length_s = flrand( 65, 70 );
	firetail->data.rocketTrail.length_e = 60;
	firetail->data.rocketTrail.hshader = cgs.media.flamejetShader;
	firetail->endTime = firetail->startTime + firetail->data.rocketTrail.life;
	VectorCopy( origin, firetail->pos.trBase );
	VectorCopy( es->pos.trDelta, firetail->pos.trDelta );
	
	// ******* SMOKE TRAIL ********
	
	step = 10;
	
	t = step * ( (startTime + step) / step );
	for ( ; t <= ent->trailTime ; t += step ) {
		BG_EvaluateTrajectory( &es->pos, t, lastPos );

		firetail = CG_AllocLocalEntity();
		firetail->leFlags = 0;
		firetail->radius = 0;
		firetail->leType = LE_ROCKET_TRAIL;
		firetail->startTime = t;
		firetail->data.rocketTrail.life = irand( 350, 400 );
		firetail->endTime = firetail->startTime + firetail->data.rocketTrail.life;
		firetail->data.rocketTrail.rgb_s[0] = flrand( 0.7059, 1 );
		firetail->data.rocketTrail.rgb_s[1] = flrand( 0.6863, 0.8392 );
		firetail->data.rocketTrail.rgb_s[2] = flrand( 0.1843, 0.3608 );
		firetail->data.rocketTrail.rgb_e[0] = flrand( 0, 0.2353 );
		firetail->data.rocketTrail.rgb_e[1] = flrand( 0, 0.2353 );
		firetail->data.rocketTrail.rgb_e[2] = flrand( 0, 0.2353 );
		firetail->data.rocketTrail.alpha_s = flrand( 0.7, 0.9 );
		firetail->data.rocketTrail.alpha_e = 0.2;
		firetail->data.rocketTrail.size_s = flrand( 1, 3 );
		firetail->data.rocketTrail.size_e = flrand( 4, 12 );
		firetail->data.rocketTrail.length_s = firetail->data.rocketTrail.length_e = flrand( 40, 55 );
		firetail->data.rocketTrail.hshader = cgs.media.smokePuffShader;
		VectorCopy( lastPos, firetail->pos.trBase );
		VectorCopy( es->pos.trDelta, firetail->pos.trDelta );
	}
	
	// ******* Other flames **********
	
	t = step * ( (startTime + step) / step );
	for ( ; t <= ent->trailTime ; t += step ) {
		BG_EvaluateTrajectory( &es->pos, t, lastPos );

		smoke = CG_SmokePuff( lastPos, up, 
					  wi->trailRadius, 
					  1, 1, 1, 0.66f,
					  wi->wiTrailTime, 
					  t,
					  0,
					  0, 
					  cgs.media.fireShaders[irand(0,2)] );
		// use the optimized local entity add
		smoke->data.rocketSmoke.life = irand( 55, 70 );
		smoke->endTime = smoke->startTime + smoke->data.rocketSmoke.life;
		smoke->data.rocketSmoke.rgb_s[0] = 1;
		smoke->data.rocketSmoke.rgb_s[1] = 1;
		smoke->data.rocketSmoke.rgb_s[2] = 1;
		smoke->data.rocketSmoke.rgb_e[0] = 1;
		smoke->data.rocketSmoke.rgb_e[1] = 1;
		smoke->data.rocketSmoke.rgb_e[2] = 1;
		smoke->data.rocketSmoke.alpha_s = flrand( 0.4, 0.7 );
		smoke->data.rocketSmoke.alpha_e = 0;
		smoke->data.rocketSmoke.size_s = flrand( 0.5, 1 );
		smoke->data.rocketSmoke.size_e = flrand( 3, 5 );
		smoke->leType = LE_ROCKET_SMOKE;
	}
	
	// ********* FLARE **********
	
	firetail = CG_AllocLocalEntity();
	firetail->radius = 0;
	firetail->endTime = cg.time + 2;
	firetail->startTime = cg.time;
	flare = &firetail->refEntity;
	flare->rotation = Q_random( &seed ) * 360;
	flare->shaderTime = ent->trailTime / 1000.0f;
	flare->renderfx = RF_RGB_TINT | RF_FORCE_ENT_ALPHA;
	VectorCopy( es->pos.trDelta, flare->origin );
	VectorNormalize( flare->origin );
	VectorScale( flare->origin, -20, flare->origin );
	VectorAdd( origin, flare->origin, flare->origin );
	flare->customShader = cgs.media.whiteFlare;
	flare->shaderRGBA[0] = 1 * 0xff;
	flare->shaderRGBA[1] = 1 * 0xff;
	flare->shaderRGBA[2] = flrand( 0.549, 0.749 ) * 0xff;
	flare->shaderRGBA[3] = flrand( 0.25, 0.35 ) * 0xff;
	VectorCopy4( flare->shaderRGBA, flare->data.sprite.vertRGBA[0] );
	VectorCopy4( flare->shaderRGBA, flare->data.sprite.vertRGBA[1] );
	VectorCopy4( flare->shaderRGBA, flare->data.sprite.vertRGBA[2] );
	VectorCopy4( flare->shaderRGBA, flare->data.sprite.vertRGBA[3] );
	flare->reType = RT_SPRITE;
	flare->radius = flrand( 24, 30 );
	firetail->leType = LE_SHOWREFENTITY;
}

void FX_RocketProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon )
{
	if( cg_fixRocket.integer == 0 ) {
		vec3_t forward;
	
		if ( VectorNormalize2( cent->currentState.pos.trDelta, forward ) == 0.0f )
		{
			forward[2] = 1.0f;
		}
	
		trap_FX_PlayEffectID( cgs.effects.rocketShotEffect, cent->lerpOrigin, forward, -1, -1 );
	} else
		CG_RocketTrail( cent, weapon );
}

/*
---------------------------
FX_RocketHitWall
---------------------------
*/

void FX_RocketHitWall( vec3_t origin, vec3_t normal )
{
	trap_FX_PlayEffectID( cgs.effects.rocketExplosionEffect, origin, normal, -1, -1 );
}

/*
---------------------------
FX_RocketHitPlayer
---------------------------
*/

void FX_RocketHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	trap_FX_PlayEffectID( cgs.effects.rocketExplosionEffect, origin, normal, -1, -1 );
}

/*
---------------------------
FX_RocketAltProjectileThink
---------------------------
*/

void FX_RocketAltProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon )
{
	if( cg_fixRocket.integer == 0 ) {
		vec3_t forward;
	
		if ( VectorNormalize2( cent->currentState.pos.trDelta, forward ) == 0.0f )
		{
			forward[2] = 1.0f;
		}
		
		trap_FX_PlayEffectID( cgs.effects.rocketShotEffect, cent->lerpOrigin, forward, -1, -1 );
	} else
		CG_RocketTrail( cent, weapon );
}
