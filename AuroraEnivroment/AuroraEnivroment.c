#include "VM_functions.h"

int load_bytecode(VM *vm, const char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        return 0;
    }

    // Чтение указателя на сегмент текста
    unsigned long text_segment_ptr;
    fread(&text_segment_ptr, sizeof(text_segment_ptr), 1, file);

    // Проверка на корректность указателя
    if (text_segment_ptr >= MEM_SIZE) {
        fprintf(stderr, "Указатель на сегмент текста выходит за пределы MEM_SIZE\n");
        fclose(file);
        return 0;
    }

    // Загрузка сегмента данных
    size_t data_segment_size = text_segment_ptr - 8;  // Размер сегмента данных
    fread(vm->mem, 1, data_segment_size, file);

    // Расчёт размера сегмента текста
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    size_t text_segment_size = file_size - text_segment_ptr;

    // Проверка на достаточность памяти для сегмента текста
    if (text_segment_size > MEM_SIZE / 4) {
        fprintf(stderr, "Недостаточно памяти для сегмента текста\n");
        fclose(file);
        return 0;
    }

    // Загрузка сегмента текста
    fseek(file, text_segment_ptr, SEEK_SET);
    fread(vm->mem + MEM_SIZE / 4, 1, text_segment_size, file);

    fclose(file);
    return 1;
}

int main(int argc, char const *argv[])
{
    // if (argc != 2)
    // {
    //     printf("Usage: ae [filename.aurbc]\n");
    //     return 1;
    // }
    
    VM vm;
    init_vm(&vm);

    if (!load_bytecode(&vm, "program.aurbc"))
    {
        free_vm(&vm);
        return 1;
    }
    // debug_print_memory(&vm);
    run_vm(&vm);

    free_vm(&vm);
    // printf("------------------------------------------------------------\n");
    // printf("|                                                          |\n");
    // printf("|                         Completed                        |\n");
    // printf("|                                                          |\n");
    // printf("------------------------------------------------------------\n");
}
