#include <iostream>
#include <png.h>

void remove_png_text_metadata(const char* filename) {
    FILE* input_file = fopen(filename, "rb");
    if (!input_file) {
        std::cerr << "Error opening input file: " << filename << std::endl;
        return;
    }

    // Создание структуры png_struct для чтения изображения
    png_structp read_png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!read_png_ptr) {
        fclose(input_file);
        std::cerr << "Error: png_create_read_struct failed" << std::endl;
        return;
    }

    // Создание структуры png_info для хранения информации о PNG-файле
    png_infop read_info_ptr = png_create_info_struct(read_png_ptr);
    if (!read_info_ptr) {
        png_destroy_read_struct(&read_png_ptr, NULL, NULL);
        fclose(input_file);
        std::cerr << "Error: png_create_info_struct failed" << std::endl;
        return;
    }

    // Установка функции обработки ошибок libpng для чтения изображения
    if (setjmp(png_jmpbuf(read_png_ptr))) {
        png_destroy_read_struct(&read_png_ptr, &read_info_ptr, NULL);
        fclose(input_file);
        std::cerr << "Error: setjmp failed" << std::endl;
        return;
    }

    // Установка файла для чтения данных PNG
    png_init_io(read_png_ptr, input_file);

    // Чтение заголовка PNG
    png_read_info(read_png_ptr, read_info_ptr);

    // Получение и удаление всех текстовых метаданных
    png_textp text_ptr;
    int num_text = 0;
    png_get_text(read_png_ptr, read_info_ptr, &text_ptr, &num_text);

    std::cout << "Found " << num_text << " text chunks." << std::endl;

    // Удаление всех текстовых метаданных
    png_set_text(read_png_ptr, read_info_ptr, nullptr, 0);

    // Получение параметров изображения
    int width = png_get_image_width(read_png_ptr, read_info_ptr);
    int height = png_get_image_height(read_png_ptr, read_info_ptr);
    png_byte color_type = png_get_color_type(read_png_ptr, read_info_ptr);
    png_byte bit_depth = png_get_bit_depth(read_png_ptr, read_info_ptr);

    // Переменные для хранения строк изображения
    png_bytep* row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
    for (int y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(read_png_ptr, read_info_ptr));
    }

    // Чтение данных изображения
    png_read_image(read_png_ptr, row_pointers);

    // Закрытие файла после чтения
    fclose(input_file);

    // Открытие файла для записи данных PNG
    FILE* output_file = fopen(filename, "wb");
    if (!output_file) {
        std::cerr << "Error opening output file: " << filename << std::endl;
        return;
    }

    // Создание структуры png_struct для записи изображения
    png_structp write_png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!write_png_ptr) {
        fclose(output_file);
        std::cerr << "Error: png_create_write_struct failed" << std::endl;
        return;
    }

    // Создание структуры png_info для записи изображения
    png_infop write_info_ptr = png_create_info_struct(write_png_ptr);
    if (!write_info_ptr) {
        png_destroy_write_struct(&write_png_ptr, NULL);
        fclose(output_file);
        std::cerr << "Error: png_create_info_struct failed" << std::endl;
        return;
    }

    // Установка функции обработки ошибок libpng для записи изображения
    if (setjmp(png_jmpbuf(write_png_ptr))) {
        png_destroy_write_struct(&write_png_ptr, &write_info_ptr);
        fclose(output_file);
        std::cerr << "Error: setjmp failed" << std::endl;
        return;
    }

    // Установка файла для записи данных PNG
    png_init_io(write_png_ptr, output_file);

    // Установка параметров изображения для записи
    png_set_IHDR(write_png_ptr, write_info_ptr, width, height, bit_depth, color_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    // Запись заголовка PNG
    png_write_info(write_png_ptr, write_info_ptr);

    // Запись данных изображения
    png_write_image(write_png_ptr, row_pointers);

    // Завершение записи PNG
    png_write_end(write_png_ptr, write_info_ptr);

    // Освобождение ресурсов
    for (int y = 0; y < height; y++) {
        free(row_pointers[y]);
    }
    free(row_pointers);
    png_destroy_write_struct(&write_png_ptr, &write_info_ptr);
    fclose(output_file);
}
int main(int argc, char **argv) {
    if(argc < 2)
    {
        printf("Usage: <filename>\n");
        return -1;
    }
    const char* filename = argv[1];//;
    remove_png_text_metadata(filename);
    std::cout << "Text metadata removed. Image saved to " << filename << std::endl;
    return 0;
}
