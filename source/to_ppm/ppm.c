#include <stdio.h>
#include <stdlib.h>

// https://imagetostl.com/jp/view-ppm-onlineで画像表示

#define WIDTH 400
#define HEIGHT 400

int main() {
    FILE *input_file = fopen("sld_result.txt", "r");
    if (input_file == NULL) {
        perror("Error opening file");
        return 1;
    }

    FILE *output_file = fopen("output.ppm", "w");
    if (output_file == NULL) {
        perror("Error creating output file");
        fclose(input_file);
        return 1;
    }

    // PPMヘッダーの出力
    fprintf(output_file, "P3\n");
    fprintf(output_file, "%d %d\n", WIDTH, HEIGHT);
    fprintf(output_file, "255\n");

    float value;
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        if (fscanf(input_file, "%f", &value) != 1) {
            fprintf(stderr, "Error reading value from file\n");
            fclose(input_file);
            fclose(output_file);
            return 1;
        }

        // 0.0から1.0の範囲の値を0から255の範囲にスケーリング
        int color_value = (int)(value * 255);
        fprintf(output_file, "%d %d %d\n", color_value, color_value, color_value); // グレースケール
    }

    fclose(input_file);
    fclose(output_file);
    return 0;
}