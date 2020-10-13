default:
	@gcc -o remove remove.c
	@gcc -o date date.c
	@gcc -o cat cat.c
	@gcc -o ls ls.c
	@gcc -o mkdir mkdir.c
	@gcc code.c
	@./a.out