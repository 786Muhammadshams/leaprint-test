desc "Print instructions"
task :help do
  puts "0. rake startup # if podman isn't up yet"
  puts "1. rake builder # if the builder container isn't running"
  puts "2. rake copy # to copy the files to the builder container"
  puts "3. rake image # build an image"
  puts "4. rake push # to push the image to the server"
end

desc "Reset podman. This helps when e.g. the podman time drifts and apt-get update doens't work."
task :reset do
  sh "/usr/local/bin/podman machine rm"
  sh "/usr/local/bin/podman machine init"
  sh "/usr/local/bin/podman machine start"
end

desc "Start up podman"
task :startup do
 sh "/usr/local/bin/podman machine start"
end

desc "Start up a development environment"
task :dev do
  if system("/usr/local/bin/podman container exists LEAPrintDev")
    sh "/usr/local/bin/podman start LEAPrintDev"
  else
    sh "/usr/local/bin/podman run --tz=local --name=LEAPrintDev --mount type=bind,source='#{pwd}',target=/app -t ubuntu:jammy"
    sh "/usr/local/bin/podman exec apt-get update"
    sh "/usr/local/bin/podman exec apt-get install -y libboost-dev libboost-locale-dev libpoco-dev libcairo2-dev curl"
  end
end

desc "Start the builder"
task :builder do
  if system("/usr/local/bin/podman container exists builder")
    sh "/usr/local/bin/podman start builder"
  else
    sh "/usr/local/bin/podman run --privileged --tz=local --name=builder -t quay.io/buildah/stable"
  end
end

desc "Copy files to builder"
task :copy do
  sh "/usr/local/bin/podman exec builder rm -rf /rootfs"
  sh "/usr/local/bin/podman exec builder mkdir /rootfs"
  sh "/usr/local/bin/podman cp Dockerfile builder:/rootfs"
  sh "/usr/local/bin/podman cp makeImage.sh builder:/rootfs"
  sh "/usr/local/bin/podman cp Server/a.out builder:/rootfs"
  sh "/usr/local/bin/podman cp Server/fonts builder:/rootfs"
  sh "say 'copy complete'"
end

desc "Build a docker image"
task :image do
  # This works but we still want to see output. Solution: redirect stdout to
  # stderr and a file, then stdout the last line of that file after the build
  # succeeded.
  #sh "podman exec -it builder /bin/bash -c 'cd /rootfs && ./makeImage.sh' | tail -1 | pbcopy"
  sh "/usr/local/bin/podman exec -it builder /bin/bash -c 'cd /rootfs && ./makeImage.sh' | tee results.txt"
  sh "tail -1 results.txt | pbcopy"
  #sh "rm -f results.txt"
  STDERR.puts "Copied to clipboard"
  #puts "^^^ Copy that hash to your clipboard!"
  sh "say 'build complete'"
end

desc "Push the new CRI-o image to github packages (ghcr.io)"
task :push do
  puts "Enter image hash (from last \"rake image\"): "
  hash = STDIN.gets.strip

  login = `podman exec -it builder buildah login --get-login ghcr.io`
  if login != "mltucker"
    puts "Login with Github PAT (with write:packagers permission)"
    puts "This PAT can be generated in Github Developer Settings"
    sh "podman exec -it builder buildah login ghcr.io"
  end

  sh "podman exec -it builder buildah push #{hash} docker://ghcr.io/comrex/tcp-test:latest"
  puts "Pushed to tcp-test"
  sh "say 'deployment complete'"
end
