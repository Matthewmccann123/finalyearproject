#include <ompt.h>
#include <omp.h>
#include <stdio.h>
#include <sys/time.h>
#include <ctime>
#include <unistd.h>
#include <time.h>
#include <timer.h>
#include <math.h>
#include <ompt-event-specific.h>
#include <ompt-initialize.h>
#include <mutex>

#define BLACK  "\x1B[0m"
#define RED  "\x1B[31m"
#define GREEN  "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define BLUE  "\x1B[34m"
#define MAGENTA  "\x1B[35m"
#define CYAN  "\x1B[36m"
#define WHITE  "\x1B[37m"

struct timeUnit{
double hours;
double minutes;
double seconds;
double milliseconds;
double microseconds;
double nanoseconds;
};

using namespace std;
static std::mutex mtx;

ompt_get_state_t ompt_get_state_fn;
ompt_function_lookup_t lookupfunction;

typedef unsigned long long timestamp_t;

static timestamp_t get_timestamp(){
struct timespec now, current;
clock_gettime(CLOCK_REALTIME, &now);
return (timestamp_t)((now.tv_sec*1000000000) + (now.tv_nsec));
}


timeUnit printTimeStamp(char* color){
	double totalnanoseconds = clock();
	volatile double hours, minutes, seconds, milliseconds, microseconds,nanoseconds;
	totalnanoseconds = get_timestamp();
	timeUnit timestamp;
	timestamp.nanoseconds = fmod(totalnanoseconds,1000.0);
	timestamp.microseconds = fmod((totalnanoseconds/1000),1000.0);
	timestamp.milliseconds = fmod((totalnanoseconds/1000000),1000.0);
	timestamp.seconds = fmod((totalnanoseconds/1000000000),60.0);
	timestamp.minutes = fmod((totalnanoseconds/60000000000),60.0);
	timestamp.hours = fmod((totalnanoseconds/3600000000000),60.0);
	printf(color);
	printf("%s%d%s%d%s%d%s%d%s%d%s%d%s","[",(int)timestamp.hours,":",(int)timestamp.minutes,":", (int)timestamp.seconds,":",(int)timestamp.milliseconds,":",(int)timestamp.microseconds,":",(int)timestamp.nanoseconds,"]");
	return timestamp;
	
}

void parallelBegin(ompt_task_id_t parent_task_id, ompt_frame_t *parent_task_frame, ompt_parallel_id_t parallel_id, uint32_t requested_team_size, void *parallel_function){
mtx.lock();
timeUnit parallelStamp = printTimeStamp("\x1B[31m");
printf("%s %d\n", " Parallel region has began ID", parallel_id);
printf(BLACK);
mtx.unlock();
}

void parallelEnd(ompt_parallel_id_t parallel_id, ompt_task_id_t taskid){
mtx.lock();
timeUnit parallelEnd = printTimeStamp("\x1B[31m");
printf("%s %d\n", "Parallel Region Ending with ID:", parallel_id);
printf(BLACK);
mtx.unlock();
}

void threadBegin(ompt_thread_id_t thread_id){
mtx.lock();
	int master = 1;
	int worker = 2;
timeUnit timestamp = printTimeStamp("\x1B[34m");
	if((long unsigned int)thread_id == master){
		printf("%s %d %s\n", "Thread", omp_get_thread_num(), "begun, This is the MASTER THREAD");
	}
	else if ((long unsigned int)thread_id == worker ){
		printf("%s %d %s\n", "Thread", omp_get_thread_num(), "begun, This is a WORKER THREAD");
	}
	else{
		printf("%s %d %s\n", "Thread", omp_get_thread_num(), "begun, This is a thread of type OTHER");
	}
printf(BLACK);
mtx.unlock();
}

void threadEnd(ompt_thread_id_t thread_id){
mtx.lock();
timeUnit timestamp = printTimeStamp("\x1B[34m");
printf("%s %d %s \n", "Thread", omp_get_thread_num(), "has ended");
printf(BLACK);
mtx.unlock();
}



void taskBegin(ompt_task_id_t parent_task_id, ompt_frame_t *parent_task_frame, ompt_task_id_t new_task_id,void *new_task_function){
mtx.lock();
timeUnit timestamp = printTimeStamp("\x1B[32m");
printf("%s %d %s\n", " Task with ID", new_task_id, "has begun");
printf(BLACK);
mtx.unlock();
}

void taskEnd(ompt_task_id_t task_id){
mtx.lock();
timeUnit timestamp = printTimeStamp("\x1B[32m"); 
printf("%s %d %s\n", " Task with ID", task_id, "has ended.");
printf(BLACK);
mtx.unlock();
}

void idleBegin(ompt_thread_id_t thread_id){
mtx.lock();
printTimeStamp("\x1B[35m");
printf("%s %d %s \n", "Thread number", omp_get_thread_num(), "is in idle state");
printf(BLACK);
mtx.unlock();
}

void idleEnd(ompt_thread_id_t thread_id){
mtx.lock();
printTimeStamp("\x1B[35m");
printf("%s %d %s \n", "Thread number", omp_get_thread_num(), "has exited idle state");
printf(BLACK);
mtx.unlock();
}

//TASKWAIT SECTION FUNCTIONS
void taskwaitBegin(ompt_task_id_t parent_task_id, ompt_frame_t *parent_task_frame, ompt_task_id_t new_task_id, void *new_task_function){
mtx.lock();
printTimeStamp("\x1B[35m");
printf("%s %d %s \n", "Thread", omp_get_thread_num(), "TASKWAITBEGIN FUNCTION");
printf(BLACK);
mtx.unlock();
}

void taskwaitEnd(ompt_task_callback_t taskCallback){
mtx.lock();
printTimeStamp("\x1B[35m");
printf("%s %d %s \n", "Thread", omp_get_thread_num(), "TASKWAITEND FUNCTION");
printf(BLACK);
mtx.unlock();
}

//TASKGROUPWAIT SECTION FUNCTIONS
void taskgroupwaitBegin(ompt_parallel_id_t parallel_id, ompt_task_id_t task_id){
mtx.lock();
printTimeStamp("\x1B[35m");
printf("%s %d %s \n", "Thread", omp_get_thread_num(), "TASKGROUPWAITBEGIN FUNCTION");
printf(BLACK);
mtx.unlock();
}
void taskgroupwaitEnd(ompt_parallel_id_t parallel_id, ompt_task_id_t task_id){
mtx.lock();
printTimeStamp("\x1B[35m");
printf("%s %d %s \n", "Thread", omp_get_thread_num(), "TASKGROUPWAITEND FUNCTION");
printf(BLACK);
mtx.unlock();
}

//BARRIER SECTION FUNCTIONS
void barrierBegin(ompt_parallel_id_t parallel_id, ompt_task_id_t task_id){
mtx.lock();
printTimeStamp("\x1B[36m");
printf("%s %d %s %d\n", "Thread", omp_get_thread_num(), "is waiting at a BARRIER, parallel region ID:",  parallel_id);
printf(BLACK);
mtx.unlock();
}
void barrierEnd(ompt_parallel_id_t parallel_id, ompt_task_id_t task_id){
mtx.lock();
printTimeStamp("\x1B[36m");
printf("%s %d %s %d\n", "Thread", omp_get_thread_num(), "has completed its BARRIER, parallel region ID:", parallel_id);
printf(BLACK);
mtx.unlock();
}

//ORDERED SECTION FUNCTIONS
void waitOrdered(ompt_wait_id_t waitId){
mtx.lock();
printTimeStamp("\x1B[36M");
ompt_state_t currentState = ompt_get_state_fn(&waitId);
printf("%s %d %s %d\n", "Thread", omp_get_thread_num(), "is currently waiting at an ordered construct. Current thread state is:", currentState);
printf(BLACK);
mtx.unlock();
}
void releaseOrdered(ompt_wait_id_t waitId){
mtx.lock();
printTimeStamp("\x1B[36M");
printf("%s %d %s\n", "Thread", omp_get_thread_num(), "has exited ordered section");
printf(BLACK);
mtx.unlock();
}
void acquireOrdered(ompt_wait_id_t waitId){
mtx.lock();
printTimeStamp("\x1B[36M");
printf("%s %d %s\n", "Thread", omp_get_thread_num(), "has entered an ordered section");
printf(BLACK);
mtx.unlock();
}

//CRITICAL SECTION FUNCTIONS
void waitCritical(ompt_wait_id_t waitId){
mtx.lock();
printTimeStamp("\x1B[31m");
printf("%s %d %s %d\n", "Thread", omp_get_thread_num(), "has reached a critical section and is waiting to enter");
printf(BLACK);
mtx.unlock();
}
void acquireCritical(ompt_wait_id_t waitId){
mtx.lock();
printTimeStamp("\x1B[31m");
printf("%s %d %s \n", "Thread", omp_get_thread_num(), "has acquired a critical section");
printf(BLACK);
mtx.unlock();
}
void releaseCritical(ompt_wait_id_t waitId){
mtx.lock();
printTimeStamp("\x1B[31m");
printf("%s %d %s \n", "Thread", omp_get_thread_num(), "has released a critical section");
printf(BLACK);
mtx.unlock();
}

//ATOMIC SECTION FUNCTIONS
void waitAtomic(ompt_wait_id_t waitId){
mtx.lock();
printTimeStamp("\x1B[31m");
printf("%s %d %s \n", "Thread", omp_get_thread_num(), "has reached an atomic section and is waiting to enter");
printf(BLACK);
mtx.unlock();
}
void acquireAtomic(ompt_wait_id_t waitId){
mtx.lock();
printTimeStamp("\x1B[31m");
printf("%s %d %s \n", "Thread", omp_get_thread_num(), "has acquired an atomic section");
printf(BLACK);
mtx.unlock();
}
void releaseAtomic(ompt_wait_id_t waitId){
mtx.lock();
printTimeStamp("\x1B[31m");
printf("%s %d %s \n", "Thread", omp_get_thread_num(), "has released an atomic section");
printf(BLACK);
mtx.unlock();
}

//LOCK SECTION FUNCTIONS
void waitLock(ompt_wait_id_t waitId){
mtx.lock();
printTimeStamp("\x1B[35m");
printf("%s %d %s \n", "Thread", omp_get_thread_num(), "has reached a locked section and is waiting to enter");
printf(BLACK);
mtx.unlock();
}
void waitfirstnestLock(ompt_wait_id_t waitId){
mtx.lock();
printTimeStamp("\x1B[35m");
printf("%s %d %s \n", "Thread", waitId, "has reached a locked section and is waiting to enter");
printf(BLACK);
mtx.unlock();
}
void acquireLock(ompt_wait_id_t waitId){
mtx.lock();
printTimeStamp("\x1B[35m");
printf("%s %d %s \n", "Thread", omp_get_thread_num(), "has acquired a lock");
printf(BLACK);
mtx.unlock();
}
void releaseLock(ompt_wait_id_t waitId){
mtx.lock();
printTimeStamp("\x1B[35m");
printf("%s %d %s \n", "Thread", omp_get_thread_num(), "has released a lock section");
printf(BLACK);
mtx.unlock();
}



extern "C" {

int ompt_initialized;

void 
ompt_initialize(ompt_function_lookup_t lookup, 
                const char *runtime_version, 
                unsigned int ompt_version)
{

  
	lookupfunction = lookup;
	ompt_set_callback_t ompt_set_callback = (ompt_set_callback_t) lookup("ompt_set_callback");
	ompt_get_state_fn = (ompt_get_state_t) lookup("ompt_get_state");
	

	//THREAD BEGIN + END //
	int threadStart = ompt_set_callback(ompt_event_thread_begin, (ompt_callback_t)threadBegin);
	ompt_set_callback(ompt_event_thread_end, (ompt_callback_t)threadEnd);

	//PARALLEL BEGIN + END//
	ompt_set_callback(ompt_event_parallel_begin, (ompt_callback_t)parallelBegin);
	ompt_set_callback(ompt_event_parallel_end, (ompt_callback_t)parallelEnd);
//	register_timer_callback(&timer, ompt_event_parallel_begin);

	//THREAD IDLE STATE BEGIN + END
	ompt_set_callback(ompt_event_idle_begin, (ompt_callback_t)idleBegin);
	ompt_set_callback(ompt_event_idle_end, (ompt_callback_t)idleEnd);

	//TASK BEGIN + END//
	ompt_set_callback(ompt_event_task_begin, (ompt_callback_t)taskBegin);
	ompt_set_callback(ompt_event_task_end, (ompt_callback_t)taskEnd);

	//BARRIER BEGIN + END
	ompt_set_callback(ompt_event_wait_barrier_begin, (ompt_callback_t)barrierBegin);
	ompt_set_callback(ompt_event_wait_barrier_end, (ompt_callback_t)barrierEnd);
	
	//TASKWAIT BEGIN + END
	ompt_set_callback(ompt_event_wait_taskwait_begin, (ompt_callback_t)taskwaitBegin);
	ompt_set_callback(ompt_event_wait_taskwait_end, (ompt_callback_t)taskwaitEnd);

	//TASKGROUPWAIT BEGIN + END
	ompt_set_callback(ompt_event_wait_taskgroup_begin, (ompt_callback_t)taskgroupwaitBegin);
	ompt_set_callback(ompt_event_wait_taskgroup_end, (ompt_callback_t)taskgroupwaitEnd);

	//WAIT LOCK/CRITICAL/ATOMIC/ORDERED/NEST
	ompt_set_callback(ompt_event_wait_lock, (ompt_callback_t)waitLock);
	ompt_set_callback(ompt_event_wait_nest_lock, (ompt_callback_t)waitfirstnestLock);
	ompt_set_callback(ompt_event_wait_critical, (ompt_callback_t)waitCritical);
	ompt_set_callback(ompt_event_wait_atomic, (ompt_callback_t)waitAtomic);
	ompt_set_callback(ompt_event_wait_ordered, (ompt_callback_t)waitOrdered);

	//RELEASE LOCK/CRITICAL/ATOMIC/ORDERED/NEST
	ompt_set_callback(ompt_event_release_lock, (ompt_callback_t)releaseLock);
		//ompt_set_callback(ompt_event_release_nest_lock_last, (ompt_callback_t)releaselastnestLock);
	ompt_set_callback(ompt_event_release_critical, (ompt_callback_t)releaseCritical);
	ompt_set_callback(ompt_event_release_atomic, (ompt_callback_t)releaseAtomic);
	ompt_set_callback(ompt_event_release_ordered, (ompt_callback_t)releaseOrdered);

	//ACQUIRE LOCK/CRITICAL/ATOMIC/ORDERED/NEST
	ompt_set_callback(ompt_event_acquired_lock, (ompt_callback_t)acquireLock);
	      //ompt_set_callback(ompt_event_acquired_nest_lock_first, (ompt_callback_t)acquirefirstnestLock);
	ompt_set_callback(ompt_event_acquired_critical, (ompt_callback_t)acquireCritical);
	ompt_set_callback(ompt_event_acquired_atomic, (ompt_callback_t)acquireAtomic);
	ompt_set_callback(ompt_event_acquired_ordered, (ompt_callback_t)acquireOrdered);

	



	  printf("ompt_initialize(lookup = %p, runtime_version = %s, "
		 "ompt_version = %d)\n", lookup, runtime_version, ompt_version);
	  ompt_initialized = 1;
}


ompt_initialize_t 
ompt_tool(void)
{
  return ompt_initialize;
}


}

int openmp_init()
{
	return omp_get_max_threads();
}



int main()
{
int nthreads, tid;
	printf("OpenMP max threads %d\n", openmp_init());
	printf("ompt_initialize %d\n", ompt_initialized);
	
	omp_lock_t lock;
	omp_init_lock(&lock);
	 #pragma omp parallel num_threads(2)
	  {	
		#pragma omp critical(firstCritical)
		{
		for(int i=0; i<1000000530; i++)
		{
			volatile int a = 50000;
			a = a * 40005343463;
		}
 			tid = omp_get_thread_num();
		  	printf("Hello World from critical section in thread = %d\n", tid);
		}

		//EACH PART OF THE SECTION CAN BE ALLOCATED TO A DIFFERENT THREAD
		#pragma omp sections
		{
		 int a = 20;
		 int b = 30;
		 volatile int c = a*b;
		}
		
		#pragma omp ordered
		{
		 int a = 20;
		 int b = 30;
		 volatile int c = a*b;
		}

		//ONLY ONE THREAD COMPLETES THIS TASK
		#pragma omp single
		{
		sleep(3);
		}

		volatile long b = 4392439043432;
		#pragma omp atomic
		b = b*4930492;
		omp_set_lock(&lock);
		for(int i=0; i<9000300; i++)
		{
			volatile int a = 50000;
			a = a * 4000;
		}
		omp_unset_lock(&lock);
		for(int i=0; i<10000530; i++)
		{
			volatile int a = 50000;
			a = a * 40005343463;
		}
		#pragma omp master
         	        {
				int a = 0;
			}
	

		#pragma omp task
		{
		int a = 5;
		int b = a*5043290432;
		sleep(21);
			#pragma omp taskwait
			{
			 sleep(1);
			}
		}
		#pragma omp taskwait
		{
		sleep(1);
		}
	  }
	for(int i=0; i<10; i++)
		{
			sleep(1);
		}
	

	return 0;
}


