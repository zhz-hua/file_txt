一：init.rc
service zygote /system/bin/app_process -Xzygote /system/bin --zygote --start-system-server                                                                                                               
    class core
    socket zygote stream 660 root system
    onrestart write /sys/android_power/request_state wake
    onrestart write /sys/power/state on
    onrestart restart media
    onrestart restart netd
    
    第一行创建了名为 zygote 的进程，这个进程是通过 app_process 的 main 启动并以”-Xzygote /system/bin –zygote –start-system-server”作为main的入口参数。
    
二：frameworks/base/cmds/app_process/app_main.cpp   init进程启动

 int main(int argc, char* const argv[])   
 {     
 		AppRuntime runtime;                                                                                                                                                        
    // Process command line arguments
    // ignore argv[0]
    argc--;
    argv++;
                                                                                                                           
    while (i < argc) {                                                                                                                                        
        const char* arg = argv[i++];                                                                                                                          
        if (!parentDir) {                                                                                                                                     
            parentDir = arg;                                                                                                                                  
        } else if (strcmp(arg, "--zygote") == 0) {                                                                                                            
            zygote = true;                                                                                                                                    
            niceName = "zygote";                                                                                                                              
        } else if (strcmp(arg, "--start-system-server") == 0) {                                                                                               
            startSystemServer = true;                                                                                                                         
        } else if (strcmp(arg, "--application") == 0) {                                                                                                       
            application = true;                                                                                                                               
        } else if (strncmp(arg, "--nice-name=", 12) == 0) {                                                                                                   
            niceName = arg + 12;                                                                                                                              
        } else {                                                                                                                                              
            className = arg;                                                                                                                                  
            break;                                                                                                                                            
        }                                                                                                                                                     
    }                                                                                                                                                         
                                                                                                                                                              
    if (niceName && *niceName) {                                                                                                                              
        setArgv0(argv0, niceName);                                                                                                                            
        set_process_name(niceName);                                                                                                                           
    }                                                                                                                                                         
                                                                                                                                                              
    runtime.mParentDir = parentDir;                                                                                                                           
                                                                                                                                                              
    if (zygote) {                                                                                                                                             
        runtime.start("com.android.internal.os.ZygoteInit",                                                                                                   
                startSystemServer ? "start-system-server" : "");                                                                                              
    }...
}
AppRuntime 继承自 AndroidRuntime，因此下一步就执行到 AndroidRuntime 的 start 函数
ntime 继承自 AndroidRuntime，因此下一步就执行到 AndroidRuntime 的 start 函数。

	void AndroidRuntime::start(const char* className, const Vector<String8>& options)
	{
	    /* start the virtual machine */ // 创建虚拟机
	    JniInvocation jni_invocation;
	    jni_invocation.Init(NULL);
	    JNIEnv* env;
	    if (startVm(&mJavaVM, &env) != 0) {
	        return;
	    }
	    onVmCreated(env);
	
	    ...
	    //调用className对应类的静态main()函数
	    char* slashClassName = toSlashClassName(className);
	    jclass startClass = env->FindClass(slashClassName);
	    jmethodID startMeth = env->GetStaticMethodID(startClass, "main",
	    env->CallStaticVoidMethod(startClass, startMeth, strArray);
	    ...
	}
	start函数主要做两件事：创建虚拟机和调用传入类名对应类的 main 函数。因此下一步就执行到com.android.internal.os.ZygoteInit 的 main 函数。
	
三  frameworks/base/core/java/com/android/internal/os/ZygoteInit.java
    public static void main(String argv[]) {                                                                                                                              
	    try {                                                                                                                                                             
	        registerZygoteSocket();                                                                                                                                       
	        ...                   	                                                                                                                                                                      
	        if (argv[1].equals("start-system-server")) {                                                                                                                  
	            startSystemServer();                                                                                                                                      
	        } else if (!argv[1].equals("")) {                                                                                                                             
	            throw new RuntimeException(argv[0] + USAGE_STRING);                                                                                                       
	        }                                                                                                                                                                                                                                                                                                                                   
	        runSelectLoop();                                                                                                                                              
	        ...                                                                                                                                                          
	        closeServerSocket();                                                                                                                                          
	    } catch (MethodAndArgsCaller caller) {                                                                                                                            
	        caller.run();                                                                                                                                                 
	    } catch (RuntimeException ex) {                                                                                                                                   
	        Log.e(TAG, "Zygote died with exception", ex);                                                                                                                 
	        closeServerSocket();                                                                                                                                          
	        throw ex;                                                                                                                                                     
	    }                                                                                                                                                                 
	}
	它主要做了三件事情:
1. 调用 registerZygoteSocket 函数创建了一个 socket 接口，用来和 ActivityManagerService 通讯；
2. 调用 startSystemServer 函数来启动 SystemServer;
3. 调用 runSelectLoop 函数进入一个无限循环在前面创建的 socket 接口上等待 ActivityManagerService 请求创建新的应用程序进程。

这里要留意 catch (MethodAndArgsCaller caller) 这一行，android 在这里通过抛出一个异常来处理正常的业务逻辑。 
 socket zygote stream 660 root system
 系统启动脚本文件 init.rc 是由 init 进程来解释执行的，
 如果其它进程也需要打开这个 /dev/socket/zygote 文件来和 zygote 进程进行通信，那就必须要通过文件名来连接这个 LocalServerSocket了。
 也就是说创建 zygote socket 之后，ActivityManagerService 就能够通过该 socket 与 zygote 进程通信从而 fork 创建新进程，
 android 中的所有应用进程都是通过这种方式 fork zygote 进程创建的。在 ActivityManagerService中 的 startProcessLocked 
 中调用了Process.start()方法，进而调用 Process.startViaZygote 和 Process.openZygoteSocketIfNeeded。    
 
  private static boolean startSystemServer()
     throws MethodAndArgsCaller, RuntimeException {
 long capabilities = posixCapabilitiesAsBits(
     OsConstants.CAP_KILL,
     OsConstants.CAP_NET_ADMIN,
     OsConstants.CAP_NET_BIND_SERVICE,
     OsConstants.CAP_NET_BROADCAST,
     OsConstants.CAP_NET_RAW,
     OsConstants.CAP_SYS_MODULE,
     OsConstants.CAP_SYS_NICE,
     OsConstants.CAP_SYS_RESOURCE,
     OsConstants.CAP_SYS_TIME,
     OsConstants.CAP_SYS_TTY_CONFIG
 );
 /* Hardcoded command line to start the system server */
 String args[] = {
     "--setuid=1000",
     "--setgid=1000",
     "--setgroups=1001,1002,1003,1004,1005,1006,1007,1008,1009,1010,1018,1032,3001,3002,3003,3006,3007",
     "--capabilities=" + capabilities + "," + capabilities,
     "--runtime-init",
     "--nice-name=system_server",                                                                                                                                  
     "com.android.server.SystemServer",                                                                                                                            
 };                                                                                                                                                                                                                                                                                                                           
            /* Request to fork the system server process */                                                                                                               
             pid = Zygote.forkSystemServer(                                                                                                                                
                     parsedArgs.uid, parsedArgs.gid,                                                                                                                       
                     parsedArgs.gids,                                                                                                                                      
                     parsedArgs.debugFlags,                                                                                                                                
                     null,                                                                                                                                                 
                     parsedArgs.permittedCapabilities,                                                                                                                     
                     parsedArgs.effectiveCapabilities);                                                                                                                    
         } catch (IllegalArgumentException ex) {                                                                                                                           
             throw new RuntimeException(ex);                                                                                                                               
         }                                                                                                                                                                    
                                                                                                                                                                           
         /* For child process */                                                                                                                                           
         if (pid == 0) {                                                                                                                                                   
             handleSystemServerProcess(parsedArgs);                                                                                                                        
         }                                                                                                                                                                 
                                                                                                                                                                           
         return true;                                                                                                                                                      
     }  
     
创建名为“system_server”的进程，其入口是： com.android.server.SystemServer 的 main 函数。
zygote 进程通过 Zygote.forkSystemServer 函数来创建一个新的进程来启动 SystemServer 组件，
返回值 pid 等 0 的地方就是新的进程要执行的路径，即新创建的进程会执行 handleSystemServerProcess 函数。

private static void handleSystemServerProcess(                                                                                                                        
             ZygoteConnection.Arguments parsedArgs)
             throws ZygoteInit.MethodAndArgsCaller {                                                                                                                       
         
         closeServerSocket();                                                                                                                                              
         
         // set umask to 0077 so new files and directories will default to owner-only permissions.
         Libcore.os.umask(S_IRWXG | S_IRWXO);
 
         if (parsedArgs.niceName != null) {
             Process.setArgV0(parsedArgs.niceName);                                                                                                                        
         }
             
         if (parsedArgs.invokeWith != null) {
             WrapperInit.execApplication(parsedArgs.invokeWith,
                     parsedArgs.niceName, parsedArgs.targetSdkVersion,
                     null, parsedArgs.remainingArgs);
         } else {
             /*
              * Pass the remaining arguments to SystemServer.
              */
             RuntimeInit.zygoteInit(parsedArgs.targetSdkVersion, parsedArgs.remainingArgs);                                                                                
         }   
 
         /* should never reach here */
     }    
handleSystemServerProcess 会抛出 MethodAndArgsCaller 异常，前面提到这个异常其实是处理正常业务逻辑的，相当于一个回调。由于由 zygote 
进程创建的子进程会继承 zygote 进程在前面创建的 socket 文件描述符，而这里的子进程又不会用到它，因此，这里就调用 closeServerSocket 函数来关闭它 
handleSystemServerProcess 函数接着调用 RuntimeInit.zygoteInit 函数来进一步执行启动

四;./frameworks/base/core/java/com/android/internal/os/RuntimeInit.java


public static final void zygoteInit(int targetSdkVersion, String[] argv)                                                                                              
             throws ZygoteInit.MethodAndArgsCaller {
         if (DEBUG) Slog.d(TAG, "RuntimeInit: Starting application from zygote");
 
         redirectLogStreams(); //// 做一些常规初始化  
 
         commonInit();
         nativeZygoteInit();   //// native层的初始化
 
         applicationInit(targetSdkVersion, argv);  // 调用应用程序java层的main 方法  
     }    
     
system_server调用完nativeZygoteInit之后，便于Binder通信系统建立了联系，这样system_server就能使用Binder通信了。

private static void applicationInit(int targetSdkVersion, String[] argv)                                                                                              
             throws ZygoteInit.MethodAndArgsCaller {                                                                                                                       
                                                                                          
         nativeSetExitWithoutCleanup(true);                                                                                                                                
                                                                                                               
         VMRuntime.getRuntime().setTargetHeapUtilization(0.75f);                                                                                                           
         VMRuntime.getRuntime().setTargetSdkVersion(targetSdkVersion);                                                                                                     
                                                                                                                                                                           
         final Arguments args;                                                                                                                                             
         try {                                                                                                                                                             
             args = new Arguments(argv);                                                                                                                                   
         } catch (IllegalArgumentException ex) {                                                                                                                           
             Slog.e(TAG, ex.getMessage());                                                                                                                                 
             // let the process exit                                                                                                                                       
             return;                                                                                                                                                       
         }                                                                                                                                                                 
                                                                                                                                                                           
         // Remaining arguments are passed to the start class's static main                                                                                                
         invokeStaticMain(args.startClass, args.startArgs);                                                                                                                
     }                                                                                                                                                                     
 MRuntime进行简单的初始化之后，invokeStaticMain函数就开始进入system_server的java层main函数中开始执行。其中，
 根据前面硬编码的启动参数可知，system_server java层的类为com.android.server.SystemServer。
invokeStaticMain函数的调用很有意思，我们接着往下看： 

private static void invokeStaticMain(String className, String[] argv)                                                                                                 
             throws ZygoteInit.MethodAndArgsCaller {                                                                                                                       
         Class<?> cl;                                                                                                                                                      
                                                                                                                                                                           
         try {                                                                                                                                                             
             cl = Class.forName(className);   // 加载com.android.server.SystemServer这个类                                                                                                                                 
         } catch (ClassNotFoundException ex) {                                                                                                                             
             throw new RuntimeException(                                                                                                                                   
                     "Missing class when invoking static main " + className,                                                                                               
                     ex);                                                                                                                                                  
         }                                                                                                                                                                 
                                                                                                                                                                           
         Method m;                                                                                                                                                         
         try {                                                                                                                                                             
             m = cl.getMethod("main", new Class[] { String[].class });    // 获得main方法                                                                                                    
         } catch (NoSuchMethodException ex) {                                                                                                                              
             throw new RuntimeException(                                                                                                                                   
                     "Missing static main on " + className, ex);                                                                                                           
         } catch (SecurityException ex) {                                                                                                                                  
             throw new RuntimeException(                                                                                                                                   
                     "Problem getting static main on " + className, ex);                                                                                                   
         }                                                                                                                                                                 
                                                                                                                                                                           
         int modifiers = m.getModifiers();                                                                                                                                 
         if (! (Modifier.isStatic(modifiers) && Modifier.isPublic(modifiers))) {     // 判断main函数的类型                                                                                        
             throw new RuntimeException(                                                                                                                                   
                     "Main method is not public and static on " + className);                                                                                              
         }                                                                                                                                                                 
                                                                                                                                                                           
         /*                                                                                                                                                                
          * This throw gets caught in ZygoteInit.main(), which responds                                                                                                    
          * by invoking the exception's run() method. This arrangement                                                                                                     
          * clears up all the stack frames that were required in setting                                                                                                   
          * up the process.                                                                                                                                                
          */                                                                                                                                                               
         throw new ZygoteInit.MethodAndArgsCaller(m, argv);                                                                                                                
     }
ZygoteInit.MethodAndArgsCaller是一个继承自Exception的类，
invokeStaticMain最后并没有直接调用com.android.server.SystemServer的main 方法，而是抛出了一个ZygoteInit.MethodAndArgsCalle类型的异常。 

ZygoteInit.main 捕获的异常
frameworks/base/core/java/com/android/internal/os/ZygoteInit.java
public static void main(String argv[]) {                                                                                                                              
     try {                                                                                                                                                             
         registerZygoteSocket();                                                                                                                                       
                                                                                                                                                         
                                                                                                                                                                       
         if (argv[1].equals("start-system-server")) {                                                                                                                  
             startSystemServer();                                                                                                                                      
         } else if (!argv[1].equals("")) {                                                                                                                             
             throw new RuntimeException(argv[0] + USAGE_STRING);                                                                                                       
         }                                                                                                                                                             
					...
  } catch (MethodAndArgsCaller caller) {                                                                                                                            
         caller.run();                                                                                                                                                 
     } catch (RuntimeException ex) {                                                                                                                                   
         Log.e(TAG, "Zygote died with exception", ex);                                                                                                                 
         closeServerSocket();                                                                                                                                          
         throw ex;                                                                                                                                                     
     }                                                                                                                                                                 
 }
 main函数中通过try-catch最终捕获到了MethodAndArgsCaller异常，并通过异常类的run函数来处理。 
     public static class MethodAndArgsCaller extends Exception                                                                                                             
            implements Runnable {                                                                                                                                         
        /** method to call */                                                                                                                                             
        private final Method mMethod;                                                                                                                                     
                                                                                                                                                                          
        /** argument array */                                                                                                                                             
        private final String[] mArgs;                                                                                                                                     
        /// 构造函数将参数保存下来                                                                                                                                                                   
        public MethodAndArgsCaller(Method method, String[] args) {                                                                                                        
            mMethod = method;                                                                                                                                             
            mArgs = args;                                                                                                                                                 
        }                                                                                                                                                                 
        // run函数调用main函数                                                                                                                                                                   
        public void run() {                                                                                                                                               
            try {                                                                                                                                                         
                mMethod.invoke(null, new Object[] { mArgs });                                                                                                             
            } catch (IllegalAccessException ex) {                                                                                                                         
                throw new RuntimeException(ex);                                                                                                                           
            } catch (InvocationTargetException ex) {                                                                                                                      
                Throwable cause = ex.getCause();                                                                                                                          
                if (cause instanceof RuntimeException) {                                                                                                                  
                    throw (RuntimeException) cause;                                                                                                                       
                } else if (cause instanceof Error) {                                                                                                                      
                    throw (Error) cause;                                                                                                                                  
                }                                                                                                                                                         
                throw new RuntimeException(ex);
            }
        }
    }
 这么复杂的跳转，其实就做了一件简单的事情：根据 className 反射调用该类的静态 main 方法。这个类名是 ZygoteInit.startSystemServer 
 方法中写死的 com.android.server.SystemServer。 从而进入 SystemServer 类的 main()方法。   
 //至此，system_server进入java世界开始执行                       