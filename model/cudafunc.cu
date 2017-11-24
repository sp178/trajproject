#include"cudafunc.h"
#define check(real,want) {if(want!=real) return -100;}
#define Mass	 36000
#define Sref	 156.0771
#define Lref	 20.29
__device__ BLOCK<2>* cuCD = nullptr;
__device__ BLOCK<2>* cuCL = nullptr;
__device__ BLOCK<2>* cuCZ = nullptr;
__device__ BLOCK<1>* curho = nullptr;
__device__ BLOCK<1>* cuma = nullptr;

__device__ InterRecod<2>* cuCDrecord[BLOCK_NUM] ;
__device__ InterRecod<2>* cuCLrecord[BLOCK_NUM] ;
__device__ InterRecod<2>* cuCZrecord[BLOCK_NUM] ;
__device__ InterRecod<1>* curhorecord;
__device__ InterRecod<1>* cumarecord;


__device__ int TrajToryConst3D(spfloat _time, const spfloat* _x, spfloat* _f, spfloat* _param, uint32_t _index)
{
	interPlatheData	interdata;
	spfloat r = _x[0], miu = _x[1], lambda = _x[2], V = _x[3], gamma = _x[4], chi = _x[5];
	spfloat B, Re;
	spfloat sigma;
	spfloat graveV, graveT;
	spfloat P;
	spfloat R[3];
	spfloat angleofdirection;			//初始发射方位角和航迹偏角夹角
	spfloat SINMIU, COSMIU, SINGAMMA, COSGAMMA, SINCHI, COSCHI, SINSIGMA, COSSIGMA;
	//spfloat	COSGAMMALOCAL;
	spfloat POWRA_R;
	POWRA_R = pow(Constant_Earth_RA / r, 2);
	SINMIU = sin(miu);
	COSMIU = cos(miu);
	SINGAMMA = sin(gamma);
	COSGAMMA = cos(gamma);
	SINCHI = sin(chi);
	COSCHI = cos(chi);
	graveV = Constant_Earth_GM / (r * r) * (1 - 1.5*Constant_Earth_J2*POWRA_R*(5 * SINMIU*SINMIU - 1));
	graveT = 3 * Constant_Earth_J2*Constant_Earth_GM / (r*r)*POWRA_R;
	B = atan2(SINMIU / COSMIU, Constant_Earth_e2);
	Re = Constant_Earth_RA*Constant_Earth_e / sqrt((SINMIU*SINMIU + Constant_Earth_e2*COSMIU*COSMIU));
	interdata.h = sqrt(r*r - Re*Re*pow(sin(B - miu), 2)) - Re*cos(B - miu);
	interdata.alpha = getAlpha(V);
	//COSGAMMALOCAL = cos(gamma + length(_param[3], _param[4], lambda, miu));
	uint32_t block = _index / THREAD_NUM;
	uint32_t thread = _index % THREAD_NUM;
	P = 0.5*interplate(curho, &interdata.h, curhorecord + _index)*Sref*V*V;
	interdata.Ma = V / interplate(cuma, &interdata.h, cumarecord + _index);
	R[0] = -P*interplate(cuCD, &interdata.alpha, cuCDrecord[block] + thread);
	R[1] = P*interplate(cuCL, &interdata.alpha, cuCLrecord[block] + thread);
	R[2] = P*interplate(cuCZ, &interdata.alpha, cuCZrecord[block] + thread);


	//	sigmamin = (GM / (r*r) - V*V / r)*Mass / R[1];
	sigma = _param[0];
	SINSIGMA = sin(sigma);
	COSSIGMA = cos(sigma);
	angleofdirection = chi - _param[1];
	_f[0] = V*SINGAMMA;
	_f[1] = V*COSGAMMA*COSCHI / r;
	_f[2] = V*COSGAMMA*SINCHI / (r*COSMIU);
	_f[3] = -R[0] / Mass - graveV *SINGAMMA \
		- graveT* SINMIU*(COSCHI*COSGAMMA*COSMIU + SINGAMMA*SINMIU)\
		- Constant_Earth_We*Constant_Earth_We * r*COSMIU*(COSCHI*COSGAMMA*SINMIU - SINGAMMA*COSMIU);
	_f[4] = R[1] / (Mass*V)*COSSIGMA - R[2] / (Mass*V)*SINGAMMA\
		- graveV *COSGAMMA / V\
		+ graveT * SINMIU*(COSCHI*SINGAMMA*COSMIU - COSGAMMA*SINMIU) / V\
		+ V*COSGAMMA / r;
	_f[5] = (-R[2] * COSSIGMA - R[1] * SINSIGMA + graveT * Mass*SINMIU*COSMIU*SINCHI \
		+ Mass*V*V * SINMIU / COSMIU*COSGAMMA*COSGAMMA* SINCHI / r) / (Mass*V*COSGAMMA);
	_f[6] = -V*COSGAMMA*cos(angleofdirection) / r;
	if ((interdata.h*Constant_Earth_g0 + 0.5*V*V) > (HIGHTMIN*Constant_Earth_g0 + 0.5*SPEEDEND*SPEEDEND)) return 0;
	else return 1;
}




__device__ int TrajTory3D(spfloat _time, const spfloat* _x, spfloat* _f, spfloat* _param, uint32_t _index)
{
	interPlatheData	interdata;
	spfloat r = _x[0], miu = _x[1], lambda = _x[2], V = _x[3], gamma = _x[4], chi = _x[5];
	spfloat B, Re;
	spfloat sigma;
	spfloat graveV, graveT;
	spfloat P;
	spfloat R[3];
	spfloat angleofdirection;			//初始发射方位角和航迹偏角夹角
	spfloat SINMIU, COSMIU, SINGAMMA, COSGAMMA, SINCHI, COSCHI, SINSIGMA, COSSIGMA;
	//spfloat	COSGAMMALOCAL;
	spfloat POWRA_R;
	POWRA_R = pow(Constant_Earth_RA / r, 2);
	SINMIU = sin(miu);
	COSMIU = cos(miu);
	SINGAMMA = sin(gamma);
	COSGAMMA = cos(gamma);
	SINCHI = sin(chi);
	COSCHI = cos(chi);
	graveV = Constant_Earth_GM / (r * r) * (1 - 1.5*Constant_Earth_J2*POWRA_R*(5 * SINMIU*SINMIU - 1));
	graveT = 3 * Constant_Earth_J2*Constant_Earth_GM / (r*r)*POWRA_R;
	B = atan2(SINMIU / COSMIU, Constant_Earth_e2);
	Re = Constant_Earth_RA*Constant_Earth_e / sqrt((SINMIU*SINMIU + Constant_Earth_e2*COSMIU*COSMIU));
	interdata.h = sqrt(r*r - Re*Re*pow(sin(B - miu), 2)) - Re*cos(B - miu);
	interdata.alpha = getAlpha(V);
	//COSGAMMALOCAL = cos(gamma + length(_param[3], _param[4], lambda, miu));
	uint32_t block = _index / THREAD_NUM;
	uint32_t thread = _index % THREAD_NUM;
	P = 0.5*interplate(curho, &interdata.h, curhorecord + _index)*Sref*V*V;
	interdata.Ma = V / interplate(cuma, &interdata.h, cumarecord + _index);
	R[0] = -P*interplate(cuCD, &interdata.alpha, cuCDrecord[block] + thread);
	R[1] = P*interplate(cuCL, &interdata.alpha, cuCLrecord[block] + thread);
	R[2] = P*interplate(cuCZ, &interdata.alpha, cuCZrecord[block] + thread);


	//	sigmamin = (GM / (r*r) - V*V / r)*Mass / R[1];
	sigma = getSigma(V, _param[0], _param[2]);
	//sigma = _param[0];
	SINSIGMA = sin(sigma);
	COSSIGMA = cos(sigma);
	angleofdirection = chi - _param[1];
	_f[0] = V*SINGAMMA;
	_f[1] = V*COSGAMMA*COSCHI / r;
	_f[2] = V*COSGAMMA*SINCHI / (r*COSMIU);
	_f[3] = -R[0] / Mass - graveV *SINGAMMA \
		- graveT* SINMIU*(COSCHI*COSGAMMA*COSMIU + SINGAMMA*SINMIU)\
		- Constant_Earth_We*Constant_Earth_We * r*COSMIU*(COSCHI*COSGAMMA*SINMIU - SINGAMMA*COSMIU);
	_f[4] = R[1] / (Mass*V)*COSSIGMA - R[2] / (Mass*V)*SINGAMMA\
		- graveV *COSGAMMA / V\
		+ graveT * SINMIU*(COSCHI*SINGAMMA*COSMIU - COSGAMMA*SINMIU) / V\
		+ V*COSGAMMA / r;
	_f[5] = (-R[2] * COSSIGMA - R[1] * SINSIGMA + graveT * Mass*SINMIU*COSMIU*SINCHI \
		+ Mass*V*V * SINMIU / COSMIU*COSGAMMA*COSGAMMA* SINCHI / r) / (Mass*V*COSGAMMA);
	_f[6] = -V*COSGAMMA*cos(angleofdirection) / r;
	if ((interdata.h*Constant_Earth_g0 + 0.5*V*V) > (HIGHTMIN*Constant_Earth_g0 + 0.5*SPEEDEND*SPEEDEND)) return 0;
	else return 1;
}
__device__ int TrajOut(spfloat _time, const spfloat* _x, spfloat* _param, uint32_t _index)
{

	if (fabs(_time - RECORDTIME) < 0.01)
	{
		spfloat r = _x[0], miu = _x[1], V = _x[3];
		spfloat h;
		spfloat B, Re;
		spfloat SINMIU, COSMIU;
		SINMIU = sin(miu);
		COSMIU = cos(miu);
		B = atan2(SINMIU / COSMIU, Constant_Earth_e2);
		Re = Constant_Earth_RA*Constant_Earth_e / sqrt((SINMIU*SINMIU + Constant_Earth_e2*COSMIU*COSMIU));
		h = sqrt(r*r - Re*Re*pow(sin(B - miu), 2)) - Re*cos(B - miu);
		_param[5] = 0.5*V*V + h*Constant_Earth_g0;
		_param[6] = _x[5] - _param[1];
	}
	return 0;
}
__global__ void InitialData(BLOCK<2>* _cuCD1,
	BLOCK<2>* _cuCL1,
	BLOCK<2>* _cuCZ1,
	BLOCK<1>* _curho1,
	BLOCK<1>* _cuma1)
{
	size_t _index1 = threadIdx.x + blockIdx.x*blockDim.x;
	if (0 == _index1)
	{
		cuCD = _cuCD1;
		cuCL = _cuCL1;
		cuCZ = _cuCZ1;
		curho = _curho1;
		cuma = _cuma1;
//		cuCDrecord = (InterRecod<2>*) malloc(THREAD_NUM*BLOCK_NUM * sizeof(InterRecod<2>));
//		cuCLrecord = (InterRecod<2>*) malloc(THREAD_NUM*BLOCK_NUM * sizeof(InterRecod<2>));
//		cuCZrecord = (InterRecod<2>*) malloc(THREAD_NUM*BLOCK_NUM * sizeof(InterRecod<2>));
		curhorecord = (InterRecod<1>*) malloc(THREAD_NUM*BLOCK_NUM * sizeof(InterRecod<1>));
		cumarecord = (InterRecod<1>*) malloc(THREAD_NUM*BLOCK_NUM * sizeof(InterRecod<1>));
	}
	__syncthreads();
};
__global__ void TrajtoryGPU(rk4_state* m_rkdata,
							Traj* _begTraj, 
							spfloat* _tarjet,
							spfloat _step, 
							spfloat _sigma, 
							spfloat _searchangle)
{
	__shared__ double sharedata_[(CUSTATANUM * 4 + CUOUTNUM+3*3)* THREAD_NUM];
	size_t _index1 = threadIdx.x + blockIdx.x*blockDim.x;
	double *data = sharedata_ + threadIdx.x*(CUSTATANUM * 4 + CUOUTNUM);

	if (_index1 >= THREAD_NUM*BLOCK_NUM) return;
	if (1 == threadIdx.x)
	{
		cuCDrecord[blockIdx.x] = (InterRecod<2>*)(sharedata_ + (CUSTATANUM * 4 + CUOUTNUM)*THREAD_NUM);
		cuCLrecord[blockIdx.x] = (InterRecod<2>*)(sharedata_ + (CUSTATANUM * 4 + CUOUTNUM+3)* THREAD_NUM);
		cuCZrecord[blockIdx.x] = (InterRecod<2>*)(sharedata_ + (CUSTATANUM * 4 + CUOUTNUM+3*2) * THREAD_NUM );
		//curhorecord[blockIdx.x] = (InterRecod<1>*)(sharedata_ + THREAD_NUM * 3 * 3);
		//cumarecord[blockIdx.x] = (InterRecod<1>*)(sharedata_ + THREAD_NUM * 3 * 4);
	}
	m_rkdata[_index1].x		=  data ;
	m_rkdata[_index1].k		=  data +CUSTATANUM;
	m_rkdata[_index1].x0	=  data +2 * CUSTATANUM;
	m_rkdata[_index1].xtmp  =  data +3 * CUSTATANUM;
	m_rkdata[_index1].param =  data  +4 * CUSTATANUM;
	memcpy(m_rkdata[_index1].x0, _begTraj, sizeof(spfloat)*CUSTATANUM);
	//m_rkdata[_index1].param[0] = deg2rad(-85);
	//m_rkdata[_index1].param[2] = 3000;
	m_rkdata[_index1].param[0] = _sigma + spfloat(_index1)*_searchangle / gridDim.x / blockDim.x;	//起始角度
	m_rkdata[_index1].param[1] = _begTraj->lanuchangle;		//内部参数 方位角
	m_rkdata[_index1].param[2] = _begTraj->V;		//起始速度
	m_rkdata[_index1].param[3] = _begTraj->begLongt;		//起始经度
	m_rkdata[_index1].param[4] = _begTraj->begLat;		//起始纬度
	m_rkdata[_index1].isDown = CalCulate(m_rkdata[_index1], _step, 0, _index1);
	__syncthreads();
	//航程差
	_tarjet[CUOUTNUM * _index1] = m_rkdata[_index1].x0[6];
	//记录纬度1
	_tarjet[CUOUTNUM * _index1 + 1] = m_rkdata[_index1].x0[1];
	//记录经度2
	_tarjet[CUOUTNUM * _index1 + 2] = m_rkdata[_index1].x0[2];
	//记录预测点和目标点距离3
	_tarjet[CUOUTNUM * _index1 + 3] = length(m_rkdata[_index1].x0[2], m_rkdata[_index1].x0[1], _begTraj->targetlambda, _begTraj->targetmiu);
	//横向误差4	
	_tarjet[CUOUTNUM * _index1 + 4] = getCrose(_begTraj->targetlambda, _begTraj->targetmiu, _begTraj->lambda, _begTraj->miu, m_rkdata[_index1].x0[2], m_rkdata[_index1].x0[1]);
	//能量5
	_tarjet[CUOUTNUM * _index1 + 5] = m_rkdata[_index1].param[5];
	//十秒后的弹道偏角
	_tarjet[CUOUTNUM * _index1 + 6] = m_rkdata[_index1].param[6];
	__syncthreads();
}; 

__global__ void InitialStata(rk4_state* m_rkdata,spfloat* _paramdata) {
	size_t _index1 = threadIdx.x + blockIdx.x*blockDim.x;

	//if (0 == _index1) {
	//	m_rkdata = (rk4_state*)malloc(BLOCK_NUM*THREAD_NUM * sizeof(rk4_state));
	//}
	//__syncthreads();
//  m_rkdata[_index1].x		=(spfloat*)malloc(sizeof(spfloat)*CUSTATANUM);
//  m_rkdata[_index1].k		=(spfloat*)malloc(sizeof(spfloat)*CUSTATANUM);
//  m_rkdata[_index1].x0	=(spfloat*)malloc(sizeof(spfloat)*CUSTATANUM);			//状态初始值
//  m_rkdata[_index1].xtmp	=(spfloat*)malloc(sizeof(spfloat)*CUSTATANUM);			//状态过渡值
//  m_rkdata[_index1].param = (spfloat*)malloc(sizeof(spfloat)*CUOUTNUM);;		//外部参数
	m_rkdata[_index1].dim = CUSTATANUM;							//7维积分
	m_rkdata[_index1].paramdim = 2;						//目标点经纬度
	m_rkdata[_index1].func = TrajTory3D;
	m_rkdata[_index1].funcOut = TrajOut;

};
__global__ void InitialStataConst(rk4_state* m_rkdata, spfloat* _paramdata) {
	size_t _index1 = threadIdx.x + blockIdx.x*blockDim.x;

	//if (0 == _index1) {
	//	m_rkdata = (rk4_state*)malloc(BLOCK_NUM*THREAD_NUM * sizeof(rk4_state));
	//}
	//__syncthreads();
	//  m_rkdata[_index1].x		=(spfloat*)malloc(sizeof(spfloat)*CUSTATANUM);
	//  m_rkdata[_index1].k		=(spfloat*)malloc(sizeof(spfloat)*CUSTATANUM);
	//  m_rkdata[_index1].x0	=(spfloat*)malloc(sizeof(spfloat)*CUSTATANUM);			//状态初始值
	//  m_rkdata[_index1].xtmp	=(spfloat*)malloc(sizeof(spfloat)*CUSTATANUM);			//状态过渡值
	//  m_rkdata[_index1].param = (spfloat*)malloc(sizeof(spfloat)*CUOUTNUM);;		//外部参数
	m_rkdata[_index1].dim = CUSTATANUM;							//7维积分
	m_rkdata[_index1].paramdim = 2;						//目标点经纬度
	m_rkdata[_index1].func = TrajToryConst3D;
	m_rkdata[_index1].funcOut = TrajOut;

};

int cudafunc::initialCudaData(BLOCK<2>* _CD, BLOCK<2>* _CL, BLOCK<2>* _CZ, BLOCK<1>* _rho, BLOCK<1>* _ma)
{
	if (isinitial) return 0;
	isinitial = true;
	cudaError_t cuerror;
	double *decordials_, *debeg_;
	check(cudaMalloc(&cuCD,sizeof(BLOCK<2>)), ::cudaSuccess);
	check(cudaMalloc(&cuCL, sizeof(BLOCK<2>)), ::cudaSuccess);
	check(cudaMalloc(&cuCZ, sizeof(BLOCK<2>)), ::cudaSuccess);
	check(cudaMalloc(&curho, sizeof(BLOCK<2>)), ::cudaSuccess);
	check(cudaMalloc(&cuma, sizeof(BLOCK<2>)), ::cudaSuccess);
	check(cudaMalloc(&cuTraj, sizeof(Traj)), ::cudaSuccess);
	check(cudaMalloc(&cuoutdata, sizeof(spfloat)*CUOUTNUM*BLOCK_NUM*THREAD_NUM), ::cudaSuccess);
	size_t _dementionlength = 0;	//坐标维度
	size_t _datalength = 1;		//数据长度
	for (size_t index = 0; index<_CD->DIM; ++index)
	{
		_dementionlength += _CD->m_cordial_demtion[index];
		_datalength *= _CD->m_cordial_demtion[index];
	}

	if (nullptr != cuCD) {
		check(cudaMalloc((void**)& decordials_, sizeof(spfloat)*_dementionlength), ::cudaSuccess);
		check(cudaMalloc((void**)& debeg_, sizeof(spfloat)*_datalength), ::cudaSuccess);
		check(cudaMemcpy(decordials_, _CD->m_cordials, sizeof(spfloat)*_dementionlength, ::cudaMemcpyHostToDevice), ::cudaSuccess);
		check(cudaMemcpy(debeg_, _CD->m_beg, sizeof(spfloat)*_datalength, ::cudaMemcpyHostToDevice), ::cudaSuccess);
		check(cudaMemcpy(cuCD->m_cordial_demtion, _CD->m_cordial_demtion, sizeof(uint32_t)*BLOCK<2>::DIM, ::cudaMemcpyHostToDevice), ::cudaSuccess);
		check(cudaMemcpy(&(cuCD->m_beg), &debeg_, sizeof(double*), ::cudaMemcpyHostToDevice), ::cudaSuccess);
		check(cudaMemcpy(&(cuCD->m_cordials), &decordials_, sizeof(double*), ::cudaMemcpyHostToDevice), ::cudaSuccess);
		debeg_ = nullptr;
		decordials_=nullptr;
	}

	_dementionlength = 0;	//坐标维度
	_datalength = 1;		//数据长度
	for (size_t index = 0; index<_CL->DIM; ++index)
	{
		_dementionlength += _CL->m_cordial_demtion[index];
		_datalength *= _CL->m_cordial_demtion[index];
	}

	if (nullptr != cuCL) {
		check(cudaMalloc((void**)& decordials_, sizeof(spfloat)*_dementionlength), ::cudaSuccess);
		check(cudaMalloc((void**)& debeg_, sizeof(spfloat)*_datalength), ::cudaSuccess);
		check(cudaMemcpy(decordials_, _CL->m_cordials, sizeof(spfloat)*_dementionlength, ::cudaMemcpyHostToDevice), ::cudaSuccess);
		check(cudaMemcpy(debeg_, _CL->m_beg, sizeof(spfloat)*_datalength, ::cudaMemcpyHostToDevice), ::cudaSuccess);
		check(cudaMemcpy(cuCL->m_cordial_demtion, _CL->m_cordial_demtion, sizeof(uint32_t)*BLOCK<2>::DIM, ::cudaMemcpyHostToDevice), ::cudaSuccess);
		check(cudaMemcpy(&(cuCL->m_beg), &debeg_, sizeof(double*), ::cudaMemcpyHostToDevice), ::cudaSuccess);
		check(cudaMemcpy(&(cuCL->m_cordials), &decordials_, sizeof(double*), ::cudaMemcpyHostToDevice), ::cudaSuccess);
		debeg_ = nullptr;
		decordials_ = nullptr;
	}

	_dementionlength = 0;	//坐标维度
	_datalength = 1;		//数据长度
	for (size_t index = 0; index<_CZ->DIM; ++index)
	{
		_dementionlength += _CZ->m_cordial_demtion[index];
		_datalength *= _CZ->m_cordial_demtion[index];
	}

	if (nullptr != cuCZ) {
		check(cudaMalloc((void**)& decordials_, sizeof(spfloat)*_dementionlength), ::cudaSuccess);
		check(cudaMalloc((void**)& debeg_, sizeof(spfloat)*_datalength), ::cudaSuccess);
		check(cudaMemcpy(decordials_, _CZ->m_cordials, sizeof(spfloat)*_dementionlength, ::cudaMemcpyHostToDevice), ::cudaSuccess);
		check(cudaMemcpy(debeg_, _CZ->m_beg, sizeof(spfloat)*_datalength, ::cudaMemcpyHostToDevice), ::cudaSuccess);
		check(cudaMemcpy(cuCZ->m_cordial_demtion, _CZ->m_cordial_demtion, sizeof(uint32_t)*BLOCK<2>::DIM, ::cudaMemcpyHostToDevice), ::cudaSuccess);
		check(cudaMemcpy(&(cuCZ->m_beg), &debeg_, sizeof(double*), ::cudaMemcpyHostToDevice), ::cudaSuccess);
		check(cudaMemcpy(&(cuCZ->m_cordials), &decordials_, sizeof(double*), ::cudaMemcpyHostToDevice), ::cudaSuccess);
		debeg_ = nullptr;
		decordials_ = nullptr;
	}

	_dementionlength = 0;	//坐标维度
	_datalength = 1;		//数据长度
	for (size_t index = 0; index<_rho->DIM; ++index)
	{
		_dementionlength += _rho->m_cordial_demtion[index];
		_datalength *= _rho->m_cordial_demtion[index];
	}

	if (nullptr != curho) {
		check(cudaMalloc((void**)& decordials_, sizeof(spfloat)*_dementionlength), ::cudaSuccess);
		check(cudaMalloc((void**)& debeg_, sizeof(spfloat)*_datalength), ::cudaSuccess);
		check(cudaMemcpy(decordials_, _rho->m_cordials, sizeof(spfloat)*_dementionlength, ::cudaMemcpyHostToDevice), ::cudaSuccess);
		check(cudaMemcpy(debeg_, _rho->m_beg, sizeof(spfloat)*_datalength, ::cudaMemcpyHostToDevice), ::cudaSuccess);
		check(cudaMemcpy(curho->m_cordial_demtion, _rho->m_cordial_demtion, sizeof(uint32_t)*BLOCK<2>::DIM, ::cudaMemcpyHostToDevice), ::cudaSuccess);
		check(cudaMemcpy(&(curho->m_beg), &debeg_, sizeof(double*), ::cudaMemcpyHostToDevice), ::cudaSuccess);
		check(cudaMemcpy(&(curho->m_cordials), &decordials_, sizeof(double*), ::cudaMemcpyHostToDevice), ::cudaSuccess);
		debeg_ = nullptr;
		decordials_ = nullptr;
	}

	_dementionlength = 0;	//坐标维度
	_datalength = 1;		//数据长度
	for (size_t index = 0; index<_ma->DIM; ++index)
	{
		_dementionlength += _ma->m_cordial_demtion[index];
		_datalength *= _ma->m_cordial_demtion[index];
	}

	if (nullptr != cuma) {
		check(cudaMalloc((void**)& decordials_, sizeof(spfloat)*_dementionlength), ::cudaSuccess);
		check(cudaMalloc((void**)& debeg_, sizeof(spfloat)*_datalength), ::cudaSuccess);
		check(cudaMemcpy(decordials_, _ma->m_cordials, sizeof(spfloat)*_dementionlength, ::cudaMemcpyHostToDevice), ::cudaSuccess);
		check(cudaMemcpy(debeg_, _ma->m_beg, sizeof(spfloat)*_datalength, ::cudaMemcpyHostToDevice), ::cudaSuccess);
		check(cudaMemcpy(cuma->m_cordial_demtion, _ma->m_cordial_demtion, sizeof(uint32_t)*BLOCK<2>::DIM, ::cudaMemcpyHostToDevice), ::cudaSuccess);
		check(cudaMemcpy(&(cuma->m_beg), &debeg_, sizeof(double*), ::cudaMemcpyHostToDevice), ::cudaSuccess);
		check(cudaMemcpy(&(cuma->m_cordials), &decordials_, sizeof(double*), ::cudaMemcpyHostToDevice), ::cudaSuccess);
		debeg_ = nullptr;
		decordials_ = nullptr;
	}

	check(cudaMalloc(&cuTraj, sizeof(Traj)),::cudaSuccess);
	check(cudaMalloc(&cuoutdata, sizeof(spfloat)*CUOUTNUM*BLOCK_NUM*THREAD_NUM),::cudaSuccess);

	InitialData <<<1, 1 >>>(cuCD, cuCL, cuCZ, curho, cuma);

	check(cudaMalloc(&m_rkdata,BLOCK_NUM*THREAD_NUM * sizeof(rk4_state)), ::cudaSuccess);
	check(cudaMalloc(&m_paramdata, BLOCK_NUM*THREAD_NUM *(CUOUTNUM+CUSTATANUM*4)*sizeof(spfloat)), ::cudaSuccess);
	InitialStata <<<BLOCK_NUM, THREAD_NUM>>> (m_rkdata, m_paramdata);
	check(cudaThreadSynchronize(), ::cudaSuccess);
	return 0;
}

int cudafunc::initialCudaDataConst(BLOCK<2>* _CD, BLOCK<2>* _CL, BLOCK<2>* _CZ, BLOCK<1>* _rho, BLOCK<1>* _ma)
{
	if (isinitial) return 0;
	isinitial = true;
	cudaError_t cuerror;
	double *decordials_, *debeg_;
	check(cudaMalloc(&cuCD, sizeof(BLOCK<2>)), ::cudaSuccess);
	check(cudaMalloc(&cuCL, sizeof(BLOCK<2>)), ::cudaSuccess);
	check(cudaMalloc(&cuCZ, sizeof(BLOCK<2>)), ::cudaSuccess);
	check(cudaMalloc(&curho, sizeof(BLOCK<2>)), ::cudaSuccess);
	check(cudaMalloc(&cuma, sizeof(BLOCK<2>)), ::cudaSuccess);
	check(cudaMalloc(&cuTraj, sizeof(Traj)), ::cudaSuccess);
	check(cudaMalloc(&cuoutdata, sizeof(spfloat)*CUOUTNUM*BLOCK_NUM*THREAD_NUM), ::cudaSuccess);
	size_t _dementionlength = 0;	//坐标维度
	size_t _datalength = 1;		//数据长度
	for (size_t index = 0; index<_CD->DIM; ++index)
	{
		_dementionlength += _CD->m_cordial_demtion[index];
		_datalength *= _CD->m_cordial_demtion[index];
	}

	if (nullptr != cuCD) {
		check(cudaMalloc((void**)& decordials_, sizeof(spfloat)*_dementionlength), ::cudaSuccess);
		check(cudaMalloc((void**)& debeg_, sizeof(spfloat)*_datalength), ::cudaSuccess);
		check(cudaMemcpy(decordials_, _CD->m_cordials, sizeof(spfloat)*_dementionlength, ::cudaMemcpyHostToDevice), ::cudaSuccess);
		check(cudaMemcpy(debeg_, _CD->m_beg, sizeof(spfloat)*_datalength, ::cudaMemcpyHostToDevice), ::cudaSuccess);
		check(cudaMemcpy(cuCD->m_cordial_demtion, _CD->m_cordial_demtion, sizeof(uint32_t)*BLOCK<2>::DIM, ::cudaMemcpyHostToDevice), ::cudaSuccess);
		check(cudaMemcpy(&(cuCD->m_beg), &debeg_, sizeof(double*), ::cudaMemcpyHostToDevice), ::cudaSuccess);
		check(cudaMemcpy(&(cuCD->m_cordials), &decordials_, sizeof(double*), ::cudaMemcpyHostToDevice), ::cudaSuccess);
		debeg_ = nullptr;
		decordials_ = nullptr;
	}

	_dementionlength = 0;	//坐标维度
	_datalength = 1;		//数据长度
	for (size_t index = 0; index<_CL->DIM; ++index)
	{
		_dementionlength += _CL->m_cordial_demtion[index];
		_datalength *= _CL->m_cordial_demtion[index];
	}

	if (nullptr != cuCL) {
		check(cudaMalloc((void**)& decordials_, sizeof(spfloat)*_dementionlength), ::cudaSuccess);
		check(cudaMalloc((void**)& debeg_, sizeof(spfloat)*_datalength), ::cudaSuccess);
		check(cudaMemcpy(decordials_, _CL->m_cordials, sizeof(spfloat)*_dementionlength, ::cudaMemcpyHostToDevice), ::cudaSuccess);
		check(cudaMemcpy(debeg_, _CL->m_beg, sizeof(spfloat)*_datalength, ::cudaMemcpyHostToDevice), ::cudaSuccess);
		check(cudaMemcpy(cuCL->m_cordial_demtion, _CL->m_cordial_demtion, sizeof(uint32_t)*BLOCK<2>::DIM, ::cudaMemcpyHostToDevice), ::cudaSuccess);
		check(cudaMemcpy(&(cuCL->m_beg), &debeg_, sizeof(double*), ::cudaMemcpyHostToDevice), ::cudaSuccess);
		check(cudaMemcpy(&(cuCL->m_cordials), &decordials_, sizeof(double*), ::cudaMemcpyHostToDevice), ::cudaSuccess);
		debeg_ = nullptr;
		decordials_ = nullptr;
	}

	_dementionlength = 0;	//坐标维度
	_datalength = 1;		//数据长度
	for (size_t index = 0; index<_CZ->DIM; ++index)
	{
		_dementionlength += _CZ->m_cordial_demtion[index];
		_datalength *= _CZ->m_cordial_demtion[index];
	}

	if (nullptr != cuCZ) {
		check(cudaMalloc((void**)& decordials_, sizeof(spfloat)*_dementionlength), ::cudaSuccess);
		check(cudaMalloc((void**)& debeg_, sizeof(spfloat)*_datalength), ::cudaSuccess);
		check(cudaMemcpy(decordials_, _CZ->m_cordials, sizeof(spfloat)*_dementionlength, ::cudaMemcpyHostToDevice), ::cudaSuccess);
		check(cudaMemcpy(debeg_, _CZ->m_beg, sizeof(spfloat)*_datalength, ::cudaMemcpyHostToDevice), ::cudaSuccess);
		check(cudaMemcpy(cuCZ->m_cordial_demtion, _CZ->m_cordial_demtion, sizeof(uint32_t)*BLOCK<2>::DIM, ::cudaMemcpyHostToDevice), ::cudaSuccess);
		check(cudaMemcpy(&(cuCZ->m_beg), &debeg_, sizeof(double*), ::cudaMemcpyHostToDevice), ::cudaSuccess);
		check(cudaMemcpy(&(cuCZ->m_cordials), &decordials_, sizeof(double*), ::cudaMemcpyHostToDevice), ::cudaSuccess);
		debeg_ = nullptr;
		decordials_ = nullptr;
	}

	_dementionlength = 0;	//坐标维度
	_datalength = 1;		//数据长度
	for (size_t index = 0; index<_rho->DIM; ++index)
	{
		_dementionlength += _rho->m_cordial_demtion[index];
		_datalength *= _rho->m_cordial_demtion[index];
	}

	if (nullptr != curho) {
		check(cudaMalloc((void**)& decordials_, sizeof(spfloat)*_dementionlength), ::cudaSuccess);
		check(cudaMalloc((void**)& debeg_, sizeof(spfloat)*_datalength), ::cudaSuccess);
		check(cudaMemcpy(decordials_, _rho->m_cordials, sizeof(spfloat)*_dementionlength, ::cudaMemcpyHostToDevice), ::cudaSuccess);
		check(cudaMemcpy(debeg_, _rho->m_beg, sizeof(spfloat)*_datalength, ::cudaMemcpyHostToDevice), ::cudaSuccess);
		check(cudaMemcpy(curho->m_cordial_demtion, _rho->m_cordial_demtion, sizeof(uint32_t)*BLOCK<2>::DIM, ::cudaMemcpyHostToDevice), ::cudaSuccess);
		check(cudaMemcpy(&(curho->m_beg), &debeg_, sizeof(double*), ::cudaMemcpyHostToDevice), ::cudaSuccess);
		check(cudaMemcpy(&(curho->m_cordials), &decordials_, sizeof(double*), ::cudaMemcpyHostToDevice), ::cudaSuccess);
		debeg_ = nullptr;
		decordials_ = nullptr;
	}

	_dementionlength = 0;	//坐标维度
	_datalength = 1;		//数据长度
	for (size_t index = 0; index<_ma->DIM; ++index)
	{
		_dementionlength += _ma->m_cordial_demtion[index];
		_datalength *= _ma->m_cordial_demtion[index];
	}

	if (nullptr != cuma) {
		check(cudaMalloc((void**)& decordials_, sizeof(spfloat)*_dementionlength), ::cudaSuccess);
		check(cudaMalloc((void**)& debeg_, sizeof(spfloat)*_datalength), ::cudaSuccess);
		check(cudaMemcpy(decordials_, _ma->m_cordials, sizeof(spfloat)*_dementionlength, ::cudaMemcpyHostToDevice), ::cudaSuccess);
		check(cudaMemcpy(debeg_, _ma->m_beg, sizeof(spfloat)*_datalength, ::cudaMemcpyHostToDevice), ::cudaSuccess);
		check(cudaMemcpy(cuma->m_cordial_demtion, _ma->m_cordial_demtion, sizeof(uint32_t)*BLOCK<2>::DIM, ::cudaMemcpyHostToDevice), ::cudaSuccess);
		check(cudaMemcpy(&(cuma->m_beg), &debeg_, sizeof(double*), ::cudaMemcpyHostToDevice), ::cudaSuccess);
		check(cudaMemcpy(&(cuma->m_cordials), &decordials_, sizeof(double*), ::cudaMemcpyHostToDevice), ::cudaSuccess);
		debeg_ = nullptr;
		decordials_ = nullptr;
	}

	check(cudaMalloc(&cuTraj, sizeof(Traj)), ::cudaSuccess);
	check(cudaMalloc(&cuoutdata, sizeof(spfloat)*CUOUTNUM*BLOCK_NUM*THREAD_NUM), ::cudaSuccess);

	InitialData << <1, 1 >> >(cuCD, cuCL, cuCZ, curho, cuma);

	check(cudaMalloc(&m_rkdata, BLOCK_NUM*THREAD_NUM * sizeof(rk4_state)), ::cudaSuccess);
	check(cudaMalloc(&m_paramdata, BLOCK_NUM*THREAD_NUM *(CUOUTNUM + CUSTATANUM * 4) * sizeof(spfloat)), ::cudaSuccess);
	InitialStataConst << <BLOCK_NUM, THREAD_NUM >> > (m_rkdata, m_paramdata);
	check(cudaThreadSynchronize(), ::cudaSuccess);
	return 0;
}




int cudafunc::TrajCaclCUDA(Traj * _beg, spfloat * _targrt, spfloat _step, spfloat _sigma, spfloat _searchangle)
{
	cudaError_t cudaStatus;
	check(cudaMemcpy(cuTraj, _beg, sizeof(Traj), ::cudaMemcpyHostToDevice), ::cudaSuccess);
	TrajtoryGPU << <BLOCK_NUM, THREAD_NUM >> > (m_rkdata,cuTraj, cuoutdata, _step, _sigma, _searchangle);
	check(cudaThreadSynchronize(), ::cudaSuccess);
	check(cudaMemcpy(_targrt, cuoutdata, sizeof(spfloat)*CUOUTNUM*BLOCK_NUM*THREAD_NUM, ::cudaMemcpyDeviceToHost), ::cudaSuccess);
	return 0;
}