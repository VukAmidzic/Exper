.intel_syntax noprefix

.data
  print_format: .asciz "%ld\n"
  scan_format: .asciz "%ld"

.text

.global main
fact:
  push rbp
  mov rbp, rsp
  sub rsp, 16
  mov QWORD PTR [rbp-8], rdi
fact_if0:
  mov rax, QWORD PTR [rbp-8]
  push rax
  mov rax, 1
  push rax
  pop rbx
  pop rax
  mov rdi, rax
  mov rsi, rbx
  call cmp_eq
  cmp rax, 1
  je fact_cond0
  jmp fact_cond1
fact_cond0:
  mov rax, 1
  jmp fact_main0
fact_cond1:
  mov rax, QWORD PTR [rbp-8]
  push rax
  mov rax, 1
  push rax
  pop rbx
  pop rax
  sub rax, rbx
  mov rdi, rax
  call fact
  push rax
  mov rax, QWORD PTR [rbp-8]
  pop rbx
  imul rax, rbx
  mov QWORD PTR [rbp-16], rax
  mov rax, QWORD PTR [rbp-16]
  jmp fact_main0
fact_main0:
  leave
  ret

main:
  push rbp
  mov rbp, rsp
  sub rsp, 16
  mov rax, 0
  mov QWORD PTR [rbp-8], rax
  lea rdi, scan_format
  lea rsi, [rbp-8]
  xor rax, rax
  call scanf
  mov rax, QWORD PTR [rbp-8]
  mov rdi, rax
  call fact
  lea rdi, print_format
  mov rsi, rax
  xor rax, rax
  call printf
  leave
  ret

