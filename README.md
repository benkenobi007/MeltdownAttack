# MeltdownAttack
Attempt to exploit the meltdown attack

## Instructions

### Kernel Module to store the data
  1. Run make to generate the Kernel module
  ```
  $make
  ```
  2. Run insmod to insert it
  ```
  $ sudo insmod KernelModule.ko
  ```
  3. Check the address of the data
  ```
  $tail /var/log/kern.log
  ```
  4. Note down the value in the statement 
  ```
  Secret data address = <value>
  ```
  5. In OutofOrderExecution.c replace the value of kernel_addr with the above.
  ```
  unsigned long kernel_addr = 0xfab3b024;
  ```
  Replace 0xfab3b024 with the value obtained.
  
### Run the attack  
  1. Compile the "OutofOrderExecution.c" file 
  ```bash
  gcc -march=native -o OutofOrder OutofOrderExecution.c
  ```
  2. Run OutofOrderExecution
  ```
  ./OutofOrder
  ```
