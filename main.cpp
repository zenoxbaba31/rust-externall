#include "plugins.h"
#include "Com.h"

#define drv_device L"\\Device\\msedge.exe"
#define drv_dos_device L"\\DosDevices\\msedge.exe"
#define drv  L"\\Driver\\msedge.exe"

PDEVICE_OBJECT driver_object;
UNICODE_STRING dev, dos;
NTSTATUS io_device_control(PDEVICE_OBJECT device, PIRP irp) {
	NTSTATUS status;
	ULONG info_size = 0;
	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(irp);
	ULONG control_code = stack->Parameters.DeviceIoControl.IoControlCode;

	switch (control_code) {

	case phy_memory: {
		pk_rw_memoryrequest in = (pk_rw_memoryrequest)irp->AssociatedIrp.SystemBuffer;
		if (!in->WRITE)
		{
			PVOID kernel_buff = ExAllocatePool(NonPagedPool, in->size);
			if (!kernel_buff) return STATUS_UNSUCCESSFUL;
			status = memory_kernel_memory((HANDLE)in->pid, in->address, kernel_buff, in->size, false);
			if (NT_SUCCESS(status)) {
				memcpy(in->output, kernel_buff, in->size);
			}
			ExFreePool(kernel_buff);
		}
		else
		{
			PVOID kernel_buff = ExAllocatePool(NonPagedPool, in->size);
			if (!kernel_buff) return STATUS_UNSUCCESSFUL;

			memcpy(kernel_buff, in->output, in->size);
			status = memory_kernel_memory((HANDLE)in->pid, in->address, kernel_buff, in->size, true);

			ExFreePool(kernel_buff);
		}
		info_size = sizeof(k_rw_memoryrequest);
	} break;

	case get_module_base: {
		pk_get_base_module_request in = (pk_get_base_module_request)irp->AssociatedIrp.SystemBuffer;
		ULONGLONG handle = get_module_adress_x64(in->pid, in->name);
		in->handle = handle;
		status = STATUS_SUCCESS;
		info_size = sizeof(k_get_base_module_request);
	} break;

	case get_base_address: {
		pk_get_base_address_request in = (pk_get_base_address_request)irp->AssociatedIrp.SystemBuffer;
		ULONGLONG handle = handle_get_base_address(in->pid);
		in->baseaddress = handle;
		status = STATUS_SUCCESS;
		info_size = sizeof(k_get_base_address_request);
	} break;

	case get_cr3: {
		pdtb_invoke in = (pdtb_invoke)irp->AssociatedIrp.SystemBuffer;

		if (!pml4::cr3_check && !pml4::pid_save) {
			pml4::savedbaseadd = handle_get_base_address((int)in->pid);
			pml4::InitializeMmPfnDatabase();
			pml4::pid_save = in->pid;
			pml4::cr3_check = true;
		}
		else if (pml4::cr3_check && pml4::pid_save != in->pid) {
			pml4::savedbaseadd = handle_get_base_address((int)in->pid);
			pml4::InitializeMmPfnDatabase();
			pml4::pid_save = in->pid;
		}
		savedptr = pml4::dirbase_from_base_address((void*)savedbaseadd);

		status = STATUS_SUCCESS;
		info_size = sizeof(dtb_invoke);
	} break;

	default:
		status = STATUS_INVALID_PARAMETER;
		info_size = 0;
		break;
	}
	irp->IoStatus.Status = status;
	irp->IoStatus.Information = info_size;
	IoCompleteRequest(irp, IO_NO_INCREMENT);
	return status;
}

NTSTATUS ioctl_create(PDEVICE_OBJECT device, PIRP irp) {
	irp->IoStatus.Status = STATUS_SUCCESS;
	irp->IoStatus.Information = 0;
	IoCompleteRequest(irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}

NTSTATUS ioctl_close(PDEVICE_OBJECT device, PIRP irp) {
	irp->IoStatus.Status = STATUS_SUCCESS;
	irp->IoStatus.Information = 0;
	IoCompleteRequest(irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}

NTSTATUS init(PDRIVER_OBJECT driver, PUNICODE_STRING path) {
	RtlInitUnicodeString(&dev, drv_device);
	RtlInitUnicodeString(&dos, drv_dos_device);

	IoCreateDevice(driver, 0, &dev, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &driver_object);
	IoCreateSymbolicLink(&dos, &dev);

	driver->MajorFunction[IRP_MJ_DEVICE_CONTROL] = io_device_control;
	driver->MajorFunction[IRP_MJ_CREATE] = ioctl_create;
	driver->MajorFunction[IRP_MJ_CLOSE] = ioctl_close;

	driver_object->Flags |= DO_DIRECT_IO;
	driver_object->Flags &= ~DO_DEVICE_INITIALIZING;

	return STATUS_SUCCESS;
}

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT driver, PUNICODE_STRING path) {
	UNREFERENCED_PARAMETER(driver);
	UNREFERENCED_PARAMETER(path);
	UNICODE_STRING drv_name;
	RtlInitUnicodeString(&drv_name, drv);
	return IoCreateDriver(&drv_name, &init);
}
