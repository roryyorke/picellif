;;; picellif.el --- send lists of files to Windows clipboard

;; Copyright (C) 2015 Rory Yorke

;; Author: Rory Yorke <rory.yorke@gmail.com>
;; Version: 0.3
;; Package-Requires: 
;; Keywords: clipboard
;; URL: http://github.com/roryyorke/picellif

;;; Commentary:

;; This package uses the picellif binary to send lists of files to the
;; Windows clipboard.  This can be useful for pasting into other applications,
;; e.g., for attaching files in Microsoft Outlook messages.

;; No change in this file from 0.1.1 to 0.2, but picellif.c has a bugfix.

;; 0.2 to 0.3: send message to user on completion of picellif invocation

;; todo: if picellif fails, show error message

(defun picellif-dired-marked-files ()
  "Send marked files (or current file, if none marked) in current
Dired buffer to picellif."
  (interactive)
  (let ((files (dired-get-marked-files)))
    (if (equal 0 
	       (apply 'call-process "picellif" nil nil nil files))
	(if (equal 1 (length files))
	    (message "%s sent to clipboard" (car files))
	  (message "%d files sent to clipboard" (length files)))
    (message "error sending files to clipboard"))))

(defun picellif-buffer-file ()
  "Send file associated with current buffer to picellif."
  (interactive)
  (when (not (buffer-file-name))
    (error "Buffer has no associated file."))
  (if (equal 0
             (call-process "picellif" nil nil nil (buffer-file-name)))
      (message "%s sent to clipboard" (buffer-file-name))
    (message "error sending %s to clipboard" (buffer-file-name))))
