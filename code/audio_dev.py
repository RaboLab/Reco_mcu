com = []
com.append("ffmpeg")
com.append("-y")
com.append("-f")
com.append("v4l2")
com.append("-thread_queue_size")
com.append("256")
com.append("-i")
com.append("/dev/video2")
com.append("-f")
com.append("alsa")
com.append("-thread_queue_size")
com.append("256")
com.append("-i")
com.append("hw:4,0")
com.append("-codec")
com.append("copy")
com.append("video.mkv")

print(com[7])