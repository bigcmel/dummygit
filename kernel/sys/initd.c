#include "initd.h"


// 该函数定义于 APP_Handler.S 汇编文件中
extern void __APP_S_jmp_to_app( BYTE* binary_base_addr );


static void initd_scheduling();

static void initd_jmp_to_app( unsigned int app_idx );

static unsigned int initd_get_empty_idx();

static void initd_clean_finished_app();



void initd_setup()
{
  short int app_idx;

  // 初始化 initd 里没有进程
  INITD_FILL_APP_NUM = INITD_EMPTY;

  // 先将所有进程的状态初始化为 未准备好
  for(app_idx=INITD_FIRST_APP_IDX ; app_idx<INITD_APP_NUM ; app_idx++)
    {
      INITD_TABLE[app_idx].status = INITD_APP_STATUS_EMPTY;
    }
}

void initd_run()
{
  if( INITD_FILL_APP_NUM != INITD_EMPTY )
    {
      initd_scheduling();

      INITD_TABLE[INITD_TOKEN].status = INITD_APP_STATUS_RUNNING;


      // 切换上下文，对应到正确的代码段
      MMU_SwitchContext( INITD_TOKEN );
      

      initd_jmp_to_app( INITD_TOKEN );

      INITD_TABLE[INITD_TOKEN].status = INITD_APP_STATUS_FINISHED;
      INITD_FILL_APP_NUM--;
    }

}

// 为全局进程表添加一个新的应用程序
unsigned int initd_register_app(BYTE* app_name,
				BYTE* app_binary_base,
				unsigned int app_binary_length)
{
  unsigned int i;
  BYTE char_tmp;
  unsigned int app_idx;
  
  i = 0;
  app_idx = initd_get_empty_idx();

  INITD_TABLE[app_idx].idx = app_idx;

  while( (char_tmp=app_name[i]) != 0 )
    {
      INITD_TABLE[app_idx].name[i] = char_tmp;
      i++;
    }

  INITD_TABLE[app_idx].name_length = i;
  
  INITD_TABLE[app_idx].binary_base = app_binary_base;

  INITD_TABLE[app_idx].binary_length = app_binary_length;


  INITD_TABLE[app_idx].status = INITD_APP_STATUS_READY;

  INITD_FILL_APP_NUM++;

  return app_idx;
}


// 先不把进程调度搞得太复杂，暂且总是指向第一个进程即可
static void initd_scheduling()
{
  INITD_TOKEN = INITD_FIRST_APP_IDX;
}

// 进入到被调度到的应用程序
static void initd_jmp_to_app( unsigned int app_idx )
{
  BYTE* binary_base_addr = INITD_TABLE[app_idx].binary_base;
  
  // 调用在 APP_Handler.S 定义的函数，进入到应用程序进程中
  __APP_S_jmp_to_app( binary_base_addr );

}

static unsigned int initd_get_empty_idx()
{
  unsigned int app_idx;
  int status_tmp;

  for(app_idx=INITD_FIRST_APP_IDX ; app_idx<INITD_APP_NUM ; app_idx++)
    {
      status_tmp = INITD_TABLE[app_idx].status;

      if( status_tmp == INITD_APP_STATUS_EMPTY )
	{
	  return app_idx;
	}
    }

  return INITD_FILL;
}

static void initd_clean_finished_app()
{
  unsigned int app_idx;
  int* status_tmp;

  for(app_idx=INITD_FIRST_APP_IDX ; app_idx<INITD_APP_NUM ; app_idx++)
    {
      status_tmp = &INITD_TABLE[app_idx].status;

      if( *status_tmp == INITD_APP_STATUS_FINISHED )
	{
	  *status_tmp = INITD_APP_STATUS_EMPTY;
	}
    }

}
