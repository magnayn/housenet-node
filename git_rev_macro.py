import subprocess

revision = (
    subprocess.check_output(["git", "rev-parse", "HEAD"])
    .strip()
    .decode("utf-8")
)
print("-DGIT_REV='\"%s\"'" % revision, end=" ")

commit_count = (
       subprocess.check_output(["git", "rev-list", "HEAD", "--count"])
    .strip()
    .decode("utf-8")
)

print("-DVERSION_NUMBER='\"1.3.%s\"'" % commit_count)
