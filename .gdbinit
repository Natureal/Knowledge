define cls
	shell clear
end
document cls
	cls clears the screen with a simple command.
end

define ib
	info breakpoints
end
document ib
	ib lists breapoints
end

define rbt
	record btrace
end

define rfh
	record function-call-history /lc
end

define rfh-simple
	record function-call-history /c
end

define rfhs
	set record function-call-history-size $arg0
end

set record function-call-history-size 30

define fo
	set follow-fork-mode $arg0
end

define fi
	finish
end

set can-use-hw-watchpoints 0

#set target-async 1
#set pagination off
#set non-stop on
