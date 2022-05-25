import os
import shutil

def copy_dir(folder_name:str, curr_time:str) -> None:
    src_dir = folder_name
    dst_dir = folder_name + '/../../static/' + curr_time
    if not os.path.exists(dst_dir):
        os.makedirs(dst_dir)
    if os.path.exists(src_dir):
        for f in os.listdir(src_dir):
            file_path = os.path.join(src_dir, f)
            dst_path = os.path.join(dst_dir, f)
            shutil.copyfile(file_path, dst_path)
