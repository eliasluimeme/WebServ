// void sendVideoChunk(int clientSocket, const std::string& chunkData, int chunkNumber) {
//     std::ostringstream response;
//     response << "HTTP/1.1 200 OK\r\n";
//     response << "Content-Type: video/mp4\r\n";
//     response << "Content-Length: " << chunkData.size() << "\r\n";
//     response << "Content-Range: bytes " << chunkNumber * CHUNK_SIZE << "-" << (chunkNumber + 1) * CHUNK_SIZE - 1 << "/*\r\n\r\n";

//     // Send the HTTP response header
//     send(clientSocket, response.str().c_str(), response.str().size(), 0);

//     // Send the chunk data
//     send(clientSocket, chunkData.c_str(), chunkData.size(), 0);
// }

void sendChunk(int clientSocket, const std::string& chunk) {
    std::ostringstream response;
    response << "HTTP/1.1 200 OK\r\n";
    response << "Content-Type: video/mp4\r\n";
    response << "Transfer-Encoding: chunked\r\n\r\n";

    // Send the HTTP response header
    send(clientSocket, response.str().c_str(), response.str().size(), 0);

    // Send the chunk size in hexadecimal format followed by the chunk data
    // std::string chunkSize = std::to_string(chunk.size()) + "\r\n";
    // send(clientSocket, chunkSize.c_str(), chunkSize.size(), 0);
    // send(clientSocket, chunk.c_str(), chunk.size(), 0);
    // send(clientSocket, "\r\n", 2, 0);
}

void Server::buildVideo(int clientSocket) {
    std::ifstream videoFile("video1.mp4", std::ios::binary);

    if (!videoFile.is_open()) {
        std::cerr << "Failed to open video file." << std::endl;
        return;
    }

    static int chunkNumber = 0;
    std::vector<char> chunkData(CHUNK_SIZE);
    char buffer[1024];
    while (!videoFile.eof()) {
        // Determine the start and end byte positions based on the Range header
        int startByte = 0; // Calculate this based on the Range header
        int endByte = startByte + sizeof(buffer) - 1; // Adjust the end byte as needed
        videoFile.seekg(startByte);
        
        // Read the chunk from the file
        videoFile.read(buffer, sizeof(buffer));
        std::string chunk(buffer, videoFile.gcount());
        sendChunk(clientSocket, chunk);
    }

    // char buffer[1024];
    // while (!videoFile.eof()) {
    //     videoFile.read(buffer, sizeof(buffer));
    //     std::string chunk(buffer, videoFile.gcount());
    //     sendChunk(clientSocket, chunk);
    // }

    // while (true) {
    //     videoFile.read(chunkData.data(), CHUNK_SIZE);
    //     int bytesRead = videoFile.gcount();

    //     if (bytesRead == 0) {
    //         std::cout << "response sent" << std::endl; // End of file
    //         break;
    //     } // TODO check if file is sent befor closing the socket

    //     // Send the current chunk
    //     // sendVideoChunk(clientSocket, std::string(chunkData.data(), bytesRead), chunkNumber);
    //     chunkNumber++;
    // }

    videoFile.close();
}

void Server::buildImage(int clientSocket) {
    std::ifstream imageFile("image1.jpg", std::ios::binary);

    if (!imageFile.is_open()) {
        std::cerr << "Failed to open image file." << std::endl;
        return;
    }

    std::vector<char> imageData((std::istreambuf_iterator<char>(imageFile)), std::istreambuf_iterator<char>());

    std::ostringstream response;
    response << "HTTP/1.1 200 OK\r\n";
    response << "Content-Type: image/jpeg\r\n";
    response << "Content-Length: " << imageData.size() << "\r\n\r\n";

    // Send the HTTP response header
    send(clientSocket, response.str().c_str(), response.str().size(), 0);

    // Send the image data
    send(clientSocket, imageData.data(), imageData.size(), 0);

    imageFile.close();
}