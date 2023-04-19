.intel_syntax noprefix

.data
  print_format: .asciz "%ld\n"
  scan_format: .asciz "%ld"

.text

.global main
main:
  push rbp
  mov rbp, rsp
#SCANS: 16
#VARS: 40
  sub rsp, 48
  mov rax, 0
  mov QWORD PTR [rbp-8], rax
  lea rdi, scan_format
  lea rsi, [rbp-8]
  xor rax, rax
  call scanf
  mov rax, 0
  mov QWORD PTR [rbp-16], rax
  mov rax, 2
  mov QWORD PTR [rbp-24], rax
loop0:
  mov rax, QWORD PTR [rbp-16]
  push rax
  mov rax, QWORD PTR [rbp-8]
  push rax
  pop rbx
  pop rax
  mov rdi, rax
  mov rsi, rbx
  call cmp_less
  cmp rax, 0
  je main0
  mov rax, 2
  mov QWORD PTR [rbp-32], rax
  mov rax, 0
  mov QWORD PTR [rbp-40], rax
loop1:
  mov rax, QWORD PTR [rbp-32]
  push rax
  mov rax, QWORD PTR [rbp-24]
  push rax
  pop rbx
  pop rax
  mov rdi, rax
  mov rsi, rbx
  call cmp_less
  cmp rax, 0
  je if1
if0:
  mov rax, QWORD PTR [rbp-24]
  push rax
  mov rax, QWORD PTR [rbp-32]
  pop rbx
  push rax
  mov rax, rbx
  pop rbx
  cqo
  xor rdx, rdx
  idiv rbx
  mov rax, rdx
  push rax
  mov rax, 0
  push rax
  pop rbx
  pop rax
  mov rdi, rax
  mov rsi, rbx
  call cmp_eq
  cmp rax, 1
  je cond0
  jmp main2
cond0:
  mov rax, QWORD PTR [rbp-40]
  push rax
  mov rax, 1
  pop rbx
  add rax, rbx
  mov QWORD PTR [rbp-40], rax
  jmp main2
main2:
  mov rax, QWORD PTR [rbp-32]
  push rax
  mov rax, 1
  pop rbx
  add rax, rbx
  mov QWORD PTR [rbp-32], rax
  jmp loop1
if1:
  mov rax, QWORD PTR [rbp-40]
  push rax
  mov rax, 0
  push rax
  pop rbx
  pop rax
  mov rdi, rax
  mov rsi, rbx
  call cmp_eq
  cmp rax, 1
  je cond1
  jmp main3
cond1:
  mov rax, QWORD PTR [rbp-24]
  lea rdi, print_format
  mov rsi, rax
  xor rax, rax
  call printf
  mov rax, QWORD PTR [rbp-16]
  push rax
  mov rax, 1
  pop rbx
  add rax, rbx
  mov QWORD PTR [rbp-16], rax
  jmp main3
main3:
  mov rax, QWORD PTR [rbp-24]
  push rax
  mov rax, 1
  pop rbx
  add rax, rbx
  mov QWORD PTR [rbp-24], rax
  jmp loop0
main0:
  leave
  ret


