#ifndef PS_ANIMATION_H
#define PS_ANIMATION_H
#include "ps_global.h"

AnimationData * GetAnimationByIndex(LinkedList *animations, u32 index);
Joint* FindJointByName(Joint **joints, u32 total, const char* name);
void UpdateAnimator(Animator *animator, float animationTime);
void UpdateVU1BoneMatrices(qword_t *q, Animator *animator, Joint **joints, u32 numJoints);
Animator *CreateAnimator(AnimationData *data);
#endif