.intel_syntax noprefix

.data
  print_format: .asciz "%ld\n"
  scan_format: .asciz "%ld"

.text

.global main
main:
  push rbp
  mov rbp, rsp
#SCANS: 32
#VARS: 32
  sub rsp, 32
  mov rax, 0
  mov QWORD PTR [rbp-8], rax
  lea rdi, scan_format
  lea rsi, [rbp-8]
  xor rax, rax
  call scanf
  mov rax, QWORD PTR [rbp-8]
  mov rbx, rax
  sal rax, 3
  mov r12, 0
  mov rdi, rax
  call malloc
  mov QWORD PTR [rbp-16], rax
arr_loop0:
  cmp r12, rbx
  je arr_next0
  mov rax, r12
  mov r8, 8
  mul r8
  mov rdx, rax
  mov rax, QWORD PTR [rbp-16]
  add rax, rdx
  mov rdi, rax
  mov rax, 0
  mov QWORD PTR [rdi], rax
  inc r12
  jmp arr_loop0
arr_next0:
  mov rax, 0
  mov QWORD PTR [rbp-24], rax
loop0:
  mov rax, QWORD PTR [rbp-24]
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
  mov rax, 0
  mov QWORD PTR [rbp-32], rax
  lea rdi, scan_format
  lea rsi, [rbp-32]
  xor rax, rax
  call scanf
  mov rax, QWORD PTR [rbp-24]
  mov r8, 8
  mul r8
  mov rdx, rax
  mov rax, QWORD PTR [rbp-16]
  add rax, rdx
  mov rdi, rax
  mov rax, QWORD PTR [rbp-32]
  push rax
  mov rax, QWORD PTR [rbp-32]
  pop rbx
  imul rax, rbx
  mov QWORD PTR [rdi], rax
  mov rax, QWORD PTR [rbp-24]
  push rax
  mov rax, 1
  pop rbx
  add rax, rbx
  mov QWORD PTR [rbp-24], rax
  jmp loop0
main0:
  mov rax, 0
  mov QWORD PTR [rbp-24], rax
loop1:
  mov rax, QWORD PTR [rbp-24]
  push rax
  mov rax, QWORD PTR [rbp-8]
  push rax
  pop rbx
  pop rax
  mov rdi, rax
  mov rsi, rbx
  call cmp_less
  cmp rax, 0
  je main1
  mov rax, QWORD PTR [rbp-24]
  mov r8, 8
  mul r8
  mov rdx, rax
  mov rax, QWORD PTR [rbp-16]
  add rax, rdx
  mov rdi, rax
  mov rax, QWORD PTR [rdi]
  lea rdi, print_format
  mov rsi, rax
  xor rax, rax
  call printf
  mov rax, QWORD PTR [rbp-24]
  push rax
  mov rax, 1
  pop rbx
  add rax, rbx
  mov QWORD PTR [rbp-24], rax
  jmp loop1
main1:
  leave
  ret


