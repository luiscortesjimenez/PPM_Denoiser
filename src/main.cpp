// Luis Cortes, CS315 Project 1

#include <iostream>
#include <string>
#include <fstream>
using namespace std;

const int NUM_IMAGES = 10;

bool verify_input_files(const string& base_name, const int image_count);
bool get_image_header(const string &base_name, string &ppm_ver, int &width, int &height, int &max_pixel);
int get_next_pixel_average(ifstream input_streams[], int num_images);
bool open_output_file(const string& base_name, ofstream& outFile);
void write_image_header(ofstream& outFile, const string& ppm_version, int image_width, int image_height, int max_pixel_value);
bool open_input_streams(const string& base_name, int image_count, ifstream input_streams[]);
void skip_stream_headers(ifstream input_streams[], int image_count);
void write_pixel_data(ofstream& outFile, ifstream input_streams[], int image_count, int image_width, int image_height);
bool generate_clean_image(const string& base_name, int image_count);

int main(int argc, char *argv[]) {
    if(argc != 2){
        cerr << "ERROR: Provide exactly one command line argument." << endl;
        return 1;
    }

    string baseName = argv[1];
    
    if(!verify_input_files(baseName, NUM_IMAGES)){
        return 1;
    }
    
    if(!generate_clean_image(baseName, NUM_IMAGES)){
        return 1;
    }

    return 0;
}

/* FUNCTION: Verifies all PPM image files opened successfully
// IN: base name string, image count
// OUT: true if all files open, false if any fail. */ 
bool verify_input_files(const string &base_name, const int image_count){
    string currentFile;

    for(int i = 0; i < image_count; i++){
        currentFile = base_name + "_noisy_" + to_string(i+1) + ".ppm";

        ifstream inFile(currentFile);

        if(!inFile.is_open()){
            cerr << "ERROR: Could not open file " << currentFile << endl;
            return false;
        }
    }

    return true;
}

/* FUNCTION: Gets PPM version, dimensions and max pixel values
// IN: base name string
// OUT: true/false, fills ppm_ver, width, height, max_pixel by ref. */
bool get_image_header(const string &base_name, string &ppm_ver, int &width, int &height, int &max_pixel){
    string currentFile = base_name + "_noisy_" + to_string(1) + ".ppm";
    ifstream inFile(currentFile);

    if(!inFile.is_open()){
        cerr << "ERROR: Could not open file " << currentFile << " to read dimensions." << endl;
        return false;
    }

    inFile >> ppm_ver;
    inFile >> width;
    inFile >> height;
    inFile >> max_pixel;

    inFile.close();
    return true;
}

/* FUNCTION: avg stream input per pixel
// IN: stream array, image count
// OUT: averaged pixel value as int. */
int get_next_pixel_average(ifstream input_streams[], int image_count){
    int sum_pixel_value = 0;
    int current_pixel_value;

    // Iterate thru streams, add all current pixel values
    for(int i =0; i < image_count; i++){
        input_streams[i] >> current_pixel_value;
        sum_pixel_value = sum_pixel_value + current_pixel_value;
    }

    // Return avg for single pixel, kind of rounding to avoid default flooring
    return (sum_pixel_value + (image_count / 2)) / image_count;
}

/* FUNCTION: Opens the output clean PPM file for writing
// IN: base name string
// OUT: true/false, opens outFile by ref */
bool open_output_file(const string& base_name, ofstream& outFile){
    string currentFile = base_name + "_clean" + ".ppm";
    outFile.open(currentFile);

    if(!outFile.is_open()){
        cerr << "ERROR: Could not create file " << currentFile << endl;
        return false;
    }
    return true;
}

/* FUNCTION: Writes PPM header info to output file
// IN: output stream, ppm version, width, height, max pixel value
// OUT: writes header to output file. */
void write_image_header(ofstream& outFile, const string& ppm_version, int image_width, int image_height, int max_pixel_value){
    outFile << ppm_version << endl;
    outFile << image_width << " " << image_height << endl;
    outFile << max_pixel_value << endl;
}

/* FUNCTION: Opens all noisy image streams for pixel reading
// IN: base name string, image count
// OUT: true/false, opens all streams by ref. */
bool open_input_streams(const string& base_name, int image_count, ifstream input_streams[]){
    string currentFile;

    for(int i = 0; i < image_count; i++){
        currentFile = base_name + "_noisy_" + to_string(i+1) + ".ppm";
        input_streams[i].open(currentFile);

        if(!input_streams[i].is_open()){
            cerr << "ERROR: Could not open stream for file: " << currentFile << endl;
            return false;
        }
    }

    return true;
}

/* FUNCTION: Skips past header data in all input streams
// IN: stream array, image count
// OUT: advances all streams to first pixel. */
void skip_stream_headers(ifstream input_streams[], int image_count){
    string junk_string;
    int junk_int;

    for(int i = 0; i < image_count; i++){
        input_streams[i] >> junk_string;
        input_streams[i] >> junk_int;
        input_streams[i] >> junk_int;
        input_streams[i] >> junk_int;
    }
}

/* FUNCTION: Iterates all pixels, writes avg values to clean image
// IN: output stream, stream array, image count, width, height
// OUT: writes all pixel data, closes output file. */
void write_pixel_data(ofstream& outFile, ifstream input_streams[], int image_count, int image_width, int image_height){
    int total_pixels = image_width * image_height * 3;

    for(int i = 0; i < total_pixels; i++){
        int avg = get_next_pixel_average(input_streams, image_count);
        outFile << avg << "\n";
    }

    outFile.close();
}

/* FUNCTION: Generates clean image from noisy image set passed
// IN: base name string, image count
// OUT: true on success, false on any failure. */
bool generate_clean_image(const string& base_name, int image_count){
    ofstream outFile;
    if(!open_output_file(base_name, outFile)){
        return false;
    }

    string ppm_version;
    int image_width, 
    image_height, 
    max_pixel_value;

    if (!get_image_header(base_name, ppm_version, image_width, image_height, max_pixel_value)) {
        cerr << "ERROR: Could not read header info." << endl;
        return false;
    }
    write_image_header(outFile, ppm_version, image_width, image_height, max_pixel_value);

    ifstream input_streams[NUM_IMAGES];
    if(!open_input_streams(base_name, image_count, input_streams)){
        return false;
    }

    skip_stream_headers(input_streams, image_count);
    write_pixel_data(outFile, input_streams, image_count, image_width, image_height);

    return true;
}