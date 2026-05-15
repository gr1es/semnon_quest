BUILD_DIR = build

all: $(BUILD_DIR)/Makefile
	$(MAKE) -C $(BUILD_DIR)

$(BUILD_DIR)/Makefile:
	cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Debug

run: all
	./semnon_quest

clean:
	$(MAKE) -C $(BUILD_DIR) clean

fclean:
	rm -rf $(BUILD_DIR) semnon_quest

re: fclean all

.PHONY: all run clean fclean re
