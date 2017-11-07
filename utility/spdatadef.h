#pragma once
#ifndef __SPDATADEF_H
#define __SPDATADEF_H
#ifndef __NVCC__

#ifndef __device__
#define __device__
#endif


#ifndef __host__
#define __host__
#endif
#else
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#endif
typedef double spfloat;
#include<stdint.h>
#define rad2deg(_x) (57.2958 * (_x))
#define deg2rad(_x) (0.0174533 * (_x))
#define Constant_PI 3.1415926535898
#define omega_E 7.292115E-5
#define R_a 6378140
#define f 0.00335281311553
#define GM 3.986005E14
#define R_b 6356755.28856
#define R_0 6371003.7814
#define g_0 9.80665
#define J_2 1.08263E-3
#define e 0.9977635403573926373497851397775
#define e2 0.99553208246651828615184517583943

#define BLOCK_NUM 4
#define THREAD_NUM 128
#define CUOUTNUM 7
#define SPEEDMAX 3000
#define SPEEDMIN 300
#define HIGHTMAX 1000000
#define HIGHTMIN 20000
#define SPEEDEND 400
#define RECORDTIME 20
#define ENERGINMAX (0.5 * SPEEDMAX * SPEEDMAX + g_0 * HIGHTMAX)
#define ENERGINMIN (0.5 * SPEEDMIN * SPEEDMIN + g_0 * HIGHTMIN)
#define CUSTATANUM 7
#define AOABEG 35
typedef int(*Derivefunc)(spfloat _time, const spfloat *_x, spfloat *_f, spfloat *_param, uint32_t _index);
typedef int(*Outfunc)(spfloat _time, const spfloat *_x, spfloat *_param, uint32_t _index);

typedef struct
{
	spfloat *x; //״̬����
	spfloat *k;
	spfloat *x0;            //״̬��ʼֵ
	spfloat *out;           //���ֵ
	spfloat *in;            //����
	spfloat *xtmp;          //״̬����ֵ
	spfloat *param;         //�ⲿ����
	Derivefunc func;        //���ֺ���
	Outfunc funcOut;        //�������
	unsigned short dim;     //����ά��
	unsigned short dimOut;  //���ά��
	unsigned short dimIn;   //����ά��
	unsigned char paramdim; //����ά��
	unsigned char isDown;   //������ɱ�־
} rk4_state;

#define CHACK_RETURN0(X) \
    {                    \
        if (X)           \
            return X;    \
    }

#endif // !



