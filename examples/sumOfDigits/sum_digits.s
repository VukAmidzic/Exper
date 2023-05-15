.intel_syntax noprefix

.data
  print_format: .asciz "%ld\n"
  scan_format: .asciz "%ld"

.text

.global main
main:
  push rbp
  mov rbp, rsp
  sub rsp, 32
  mov rax, 0
  mov QWORD PTR [rbp-8], rax
  lea rdi, scan_format
  lea rsi, [rbp-8]
  xor rax, rax
  call scanf
  mov rax, 0
  mov QWORD PTR [rbp-16], rax
loop0:
  mov rax, QWORD PTR [rbp-8]
  push rax
  mov rax, 0
  push rax
  pop rbx
  pop rax
  mov rdi, rax
  mov rsi, rbx
  call cmp_neq
  cmp rax, 0
  je main0
  mov rax, QWORD PTR [rbp-8]
  push rax
  mov rax, 10
  pop rbx
  push rax
  mov rax, rbx
  pop rbx
  cqo
  xor rdx, rdx
  idiv rbx
  mov rax, rdx
  mov QWORD PTR [rbp-24], rax
  mov rax, QWORD PTR [rbp-16]
  push rax
  mov rax, QWORD PTR [rbp-24]
  pop rbx
  add rax, rbx
  mov QWORD PTR [rbp-16], rax
  mov rax, QWORD PTR [rbp-8]
  push rax
  mov rax, 10
  pop rbx
  push rax
  mov rax, rbx
  pop rbx
  cqo
  xor rdx, rdx
  idiv rbx
  mov QWORD PTR [rbp-8], rax
  jmp loop0
main0:
  mov rax, QWORD PTR [rbp-16]
  lea rdi, print_format
  mov rsi, rax
  xor rax, rax
  call printf
  leave
  ret

