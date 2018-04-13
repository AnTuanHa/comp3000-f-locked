#!/bin/bash

ESTDERR=$((256 - 1))
EWRONG_PASSWORD=$((256 - 2))
EFILE_NOT_EXISTS=$((256 - 3))
EFILE_NOT_OWNED_BY_USER=$((256 - 4))
EFILE_CHANGE_PERMISSIONS=$((256 - 5))
EFILE_SET_XATTR=$((256 - 6))
EFILE_GET_XATTR=$((256 - 7))
EFILE_RM_XATTR=$((256 - 8))

zenity --password --title="Flock" --text="Please enter your password" | flocked $1
ERROR=$?

case ${ERROR} in
${ESTDERR})
	zenity --error --text="Failed to run flocked."
	;;
${EWRONG_PASSWORD})
	zenity --error --text="Wrong password.\nFailed to unlock file '$1'."
	;;
${EFILE_NOT_EXISTS})
	zenity --error --text="File '$1' does not exist."
	;;
${EFILE_NOT_OWNED_BY_USER})
	zenity --error --text="File '$1' is not owned by current user!\nAborting file locking."
	;;
${EFILE_CHANGE_PERMISSIONS})
	zenity --error --text="Failed to change file permissions on '$1'."
	;;
${EFILE_SET_XATTR})
	zenity --error --text="Failed to set extended attributes on '$1'"
	;;
${EFILE_GET_XATTR})
	zenity --error --text="Failed to get extended attributes on '$1'"
	;;
${EFILE_RM_XATTR})
	zenity --error --text="Failed to remove extended attributes on '$1'"
	;;
esac
