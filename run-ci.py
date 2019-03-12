import argparse
import os
import sys

from path import Path

import ci
import ci.android
import ci.cpp
import ci.ios
import ci.mail
import ci.git


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--isolate-conan-user-home", action="store_true", dest="home_isolation", default=False)
    subparsers = parser.add_subparsers(title="subcommands", dest="command")

    build_and_test_parser = subparsers.add_parser("build-and-test")
    build_and_test_parser.add_argument("--profile", required=True)
    build_and_test_parser.add_argument("--coverage", action="store_true")

    subparsers.add_parser("deploy")
    subparsers.add_parser("mirror")

    args = parser.parse_args()
    if args.home_isolation:
        ci.cpp.set_home_isolation()

    ci.cpp.update_conan_config()
    if args.command == "build-and-test":
        ci.cpp.check(args.profile, coverage=args.coverage, run_tests=True)
    elif args.command == "deploy":
        git_tag = os.environ["CI_COMMIT_TAG"]
        version = ci.version_from_git_tag(git_tag)
        ci.bump_files(version)
        ci.cpp.build_recipe(
            Path.getcwd(),
            conan_reference=f"tanker/{version}@tanker/stable",
            upload=True,
        )
    elif args.command == "mirror":
        ci.git.mirror(github_url="git@github.com:TankerHQ/mockaron")
    else:
        parser.print_help()
        sys.exit(1)


if __name__ == "__main__":
    main()
