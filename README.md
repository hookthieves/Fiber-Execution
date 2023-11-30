# Fiber-Execution
## Information
The intention of this project was to discover ways to execute a payload. Usually uzilizing threads is the primary mechanism used for execution functions and tasks within a process. I have added my own implementation into this by adding a calc.ico as a resource instead of the .text section of a PE Header. Saving a payload into the .rsrc section is a cleaner method, since larger paylaods cannot be stored in .data or r.data due to their size limitations - Which also leads to errors from Visual Studio during compliation.

## Fiber Execution Logic
1.  ConvertThreadToFiber - Converts the current thread into a fiber. You must convert a thread into a fiber before you can schedule other fibers. This is required because only fibers can execute other fibers. Therefore, if a thread needs to execute a fiber, it must first call ConvertThreadToFiber.
2.  CreateFiber - Creates the fiber object, assigns its stack and sets it up for execution. However, CreateFiber does not schedule the fiber for execution.
3.  SwitchToFiber - Schedules a fiber for execution. SwitchToFiber will only work when called from a fiber and will not work if it is called from a thread.
4.  ConvertFiberToThread - Converts the current fiber into a thread.
5.  DeleteFiber - Deletes an existing fiber.
6.  LoadResource - Loads the resource into memory
7.  LockResource - Obtains a pointer to the beginning of the resource data
8.  VirtualAlloc - Creates a temporary memory region and changing permissions the execute
9.  Memcpy - Copies payload into the memory region

## OPSEC
Currently this has opsec concerns as VirtualAlloc and Memcpy has been used to tackle the memory mapping issues. To improve on this, the operator must use stealthier techniques to move around this. IAT Hiding and Obfuscation techniques like API Hashing or the use of NT WinAPIs might be more opsec safe.

## Usage
The operator can change the payload. Please make sure that the RDATA identifier is the same in main.c and resource.h. Also determine that the .ico file is in the correct directly which is listed in resource.rc file

## Licence
MIT Licence @ [MIT](https://raw.githubusercontent.com/hookthieves/Fiber-Execution/main/LICENCE)

## Output
![image](https://github.com/hookthieves/Fiber-Execution/assets/46670348/cb5bb898-e521-4736-997b-d7455aedc7aa)

