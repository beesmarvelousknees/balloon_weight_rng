# File: balloon_weight.Dockerfile

# Use the official Debian Bookworm image
FROM debian:bookworm

# Non-interactive installs
ENV DEBIAN_FRONTEND=noninteractive

# Update APT sources to include contrib, non-free, and non-free-firmware
# Need this to install gstreamer1.0-plugins-good
RUN echo 'Types: deb\nURIs: http://deb.debian.org/debian\nSuites: bookworm bookworm-updates\nComponents: main contrib non-free non-free-firmware\nSigned-By: /usr/share/keyrings/debian-archive-keyring.gpg\n' > /etc/apt/sources.list.d/debian.sources && \
    echo 'Types: deb\nURIs: http://deb.debian.org/debian-security\nSuites: bookworm-security\nComponents: main contrib non-free non-free-firmware\nSigned-By: /usr/share/keyrings/debian-archive-keyring.gpg\n' >> /etc/apt/sources.list.d/debian.sources

# Install necessary packages
RUN apt-get update && apt-get install -y \
    build-essential \
    libssl-dev \
    vim \
    nodejs \
    libopencv-dev \
    # Need this for gstreamer to actually work, or else it defaults to something else without warning.
    gstreamer1.0-plugins-good \
    && rm -rf /var/lib/apt/lists/*

# Create a directory for the application
WORKDIR /app

# Copy the source files and scripts to the container
COPY capture.cpp /app/
COPY utils.h /app/
COPY compile.sh /app/
COPY make_cert.sh /app/
COPY balloon_weight_server.js /app/
COPY config.cfg /app/

# Make the compile and cert scripts executable
RUN chmod +x /app/compile.sh /app/make_cert.sh

# Compile the C++ program
RUN ./compile.sh

# Create HTTPS certificate and key. Dash doesn't understand source only bash does.
RUN bash ./make_cert.sh

# Command to run the Node.js server
CMD ["node", "/app/balloon_weight_server.js"]
