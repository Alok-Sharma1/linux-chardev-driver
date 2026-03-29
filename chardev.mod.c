#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xa61fd7aa, "__check_object_size" },
	{ 0x092a35a2, "_copy_from_user" },
	{ 0x37031a65, "__register_chrdev" },
	{ 0x653aa194, "class_create" },
	{ 0xe486c4b7, "device_create" },
	{ 0xc1e6c71e, "__mutex_init" },
	{ 0x52b15b3b, "__unregister_chrdev" },
	{ 0xa1dacb42, "class_destroy" },
	{ 0x1595e410, "device_destroy" },
	{ 0xd272d446, "__fentry__" },
	{ 0xf46d5bf3, "mutex_unlock" },
	{ 0xe8213e80, "_printk" },
	{ 0xd272d446, "__x86_return_thunk" },
	{ 0x357aaab3, "mutex_trylock" },
	{ 0x092a35a2, "_copy_to_user" },
	{ 0x546c19d9, "validate_usercopy_range" },
	{ 0xbebe66ff, "module_layout" },
};

static const u32 ____version_ext_crcs[]
__used __section("__version_ext_crcs") = {
	0xa61fd7aa,
	0x092a35a2,
	0x37031a65,
	0x653aa194,
	0xe486c4b7,
	0xc1e6c71e,
	0x52b15b3b,
	0xa1dacb42,
	0x1595e410,
	0xd272d446,
	0xf46d5bf3,
	0xe8213e80,
	0xd272d446,
	0x357aaab3,
	0x092a35a2,
	0x546c19d9,
	0xbebe66ff,
};
static const char ____version_ext_names[]
__used __section("__version_ext_names") =
	"__check_object_size\0"
	"_copy_from_user\0"
	"__register_chrdev\0"
	"class_create\0"
	"device_create\0"
	"__mutex_init\0"
	"__unregister_chrdev\0"
	"class_destroy\0"
	"device_destroy\0"
	"__fentry__\0"
	"mutex_unlock\0"
	"_printk\0"
	"__x86_return_thunk\0"
	"mutex_trylock\0"
	"_copy_to_user\0"
	"validate_usercopy_range\0"
	"module_layout\0"
;

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "37D4BE5FAB2AAF2F1B94A7C");
