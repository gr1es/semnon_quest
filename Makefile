all:
	make -s --no-print-directory -C build

clean:
	make -s --no-print-directory -C build clean

fclean:
	rm -rf build
	mkdir build
	cd build && cmake ..

re: fclean all

run: all
	./build/semnon_quest

.PHONY: all clean fclean re run
