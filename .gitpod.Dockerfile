FROM gitpod/workspace-full
                    
USER gitpod

# Install packages
RUN sudo apt-get update \
    && sudo apt-get install -yq \
        python3 \
        python3-pip \
        python3-setuptools \
        python3-wheel \
        ninja-build \
        valgrind \
        ccache \
    && sudo apt-get clean && sudo rm -rf /var/lib/apt/lists/* /tmp/*
RUN sudo pip3 install meson gcovr
RUN sudo /usr/sbin/update-ccache-symlinks
