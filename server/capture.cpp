#include <iostream>
#include <opencv2/opencv.hpp>
#include <openssl/evp.h>
#include "utils.h"

int main(int argc, char** argv) {
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <num_bytes_requested>\n";
        return 1;
    }

    // Parse the user's requested num bytes.
    size_t num_bytes_requested = std::stoul(argv[1]);

    // Allocate a raw array for the SHAKE256 result.
    unsigned char* output_buffer = new unsigned char[num_bytes_requested];

    // Open the webcam using GStreamer.
    cv::VideoCapture cap(0, cv::CAP_GSTREAMER);
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open video stream.\n";
        return 1;
    }

    // Capture one frame.
    cv::Mat frame;
    cap.read(frame);

    // Release the camera since we only need one frame.
    cap.release();

    // Use SHAKE256 to hash the image data.
    unsigned char* image_data = frame.data;
    size_t num_image_bytes = frame.step[0] * frame.rows;

    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    
    EVP_DigestInit_ex(mdctx, EVP_shake256(), nullptr);
    EVP_DigestUpdate(mdctx, image_data, num_image_bytes);
    
    // Finalize the digest output, specifying the desired number of bytes.
    EVP_DigestFinalXOF(mdctx, output_buffer, num_bytes_requested);

    EVP_MD_CTX_free(mdctx);

    // Convert the output to a hex string to display.
    std::string output_hex = Utils::bytes_to_hex(output_buffer, num_bytes_requested);

    // Print the resulting hash in hex.
    std::cout << output_hex << std::endl;

    // Clean up the dynamically allocated memory.
    delete[] output_buffer;

    return 0;
}
