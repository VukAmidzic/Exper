#fact->n
#FOUND ARG n IN ARGS
#FOUND ARG n IN ARGS
#fact has 1 args
#NUMBER OF FUNCS: 1
.intel_syntax noprefix

.data
  print_format: .asciz "%ld\n"
  scan_format: .asciz "%ld"

.text

.global main
fact:
  push rbp
  mov rbp, rsp
  sub rsp,8
if0:
  mov rax, rdi
  push rax
  mov rax, 1
  push rax
  pop rbx
  pop rax
  mov rdi, rax
  mov rsi, rbx
  call cmp_eq
  cmp rax, 1
  je cond0
  jmp cond1
cond0:
  mov rax, 1
  jmp main0
cond1:
  push rdi
  mov rax, rdi
  push rax
  mov rax, 1
  push rax
  pop rbx
  pop rax
  sub rax, rbx
  mov rdi, rax
  call fact
  pop rdi
  push rax
  mov rax, rdi
  pop rbx
  imul rax, rbx
  mov QWORD PTR [rbp-8], rax
  mov rax, QWORD PTR [rbp-8]
  jmp main0
main0:
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

