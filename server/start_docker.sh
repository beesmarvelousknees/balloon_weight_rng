source config.cfg 

docker buildx build -t balloon_weight_image -f balloon_weight.Dockerfile .
docker run -d --name balloon_weight_container --device=/dev/video0:/dev/video0 -p $PORT:$PORT --restart unless-stopped balloon_weight_image
