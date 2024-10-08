### Add soundplaying to engine using OpenAL: https://github.com/kcat/openal-soft
### Animation loading (Script/App)
<p>Windows:
void start()
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    if( argc != 2 )
    {
        printf("Usage: %s [cmdline]\n", argv[0]);
        return;
    }

    // Start the child process. 
    if( !CreateProcess( NULL,   // No module name (use command line)
        argv[1],        // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi )           // Pointer to PROCESS_INFORMATION structure
    ) 
    {
        printf( "CreateProcess failed (%d).\n", GetLastError() );
        return;
    }

    // Wait until child process exits.
    WaitForSingleObject( pi.hProcess, INFINITE );

    // Close process and thread handles. 
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
}
</p>
<p>Linux:
void start(){
pid_t pid,p;
    int i,status;
    printf("Parent process starts with PID = %d it's Parent ID %d\n",(int)getpid(),(int)getppid());
    
    if((pid = fork())==-1) {
        fprintf(stderr,"FORK FAILED\n");
        return -1;
    }
    if(pid == 0 ) {
        printf("Child process starts with PID = %d\n",(int)getpid());
        for(i = 0 ; i < 5; i++) {
            printf("Child prints [%d]\n",i);
            sleep(2);
        }
        _exit(0);
        //exit(0);
    }
    else {
        p  = wait(&status);
        printf("Parent resumes execution, took control from the child %d \n",(int)p);
        //printf("Return status of the child is %d\n",status);
        for(i = 0; i< 5 ; i++) {
            sleep(2);
            printf("Parent prints [%d]\n",i);
            //sleep(2);
        }
        _exit(0);
    }
}
</p>
<p>
    Animation file would consist of a header info 
        1byte = size of animation name in bytes
        n bytes = animation name
        1byte = how many frames
        3bytes = frame_header[BEG]
        n bytes = frame
        3bytes = frame_footer[END]
    frames would be saved to an unsigned char* frames[frame_count];
    then would be loaded using stbi_load_from_memory which then is passed to textureatlas
    Change Sprite class to load frames ngesprite files instead of png
    Name of spirte is determined depending on the header in the ngesprite file.

    All pngs will be parsed into ngesprite files using the tool, and will automatically be loaded when running the app
</p>
### Add collision boxes