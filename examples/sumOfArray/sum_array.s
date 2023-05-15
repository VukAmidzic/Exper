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
  mov rax, QWORD PTR [rbp-8]
  mov rdi, rax
  mov rax, 0
  mov rsi, rax
  call dyn_malloc
  mov QWORD PTR [rbp-16], rax
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
  mov rdi, QWORD PTR [rbp-16]
  mov rax, QWORD PTR [rbp-24]
  mov rsi, rax
  mov rax, QWORD PTR [rbp-32]
  push rax
  mov rax, QWORD PTR [rbp-32]
  pop rbx
  imul rax, rbx
  mov rdx, rax
  call set
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
  mov rdi, QWORD PTR [rbp-16]
  mov rax, QWORD PTR [rbp-24]
  mov rsi, rax
  call get
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

