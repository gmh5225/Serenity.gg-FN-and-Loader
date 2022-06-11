#pragma once

bool while_rotating;
uint64_t OldMesh;
bool OldRotation;
int increased_value;
int decreased_value;
bool onceB = true;

class AFortItemDefinition {
public:
	FText* DisplayName() {
		return reinterpret_cast<FText*>(*(uintptr_t*)((uintptr_t)this + 0xfb8));
	}

	UINT8 Tier() {
		return read<UINT8>((uintptr_t)this + 0x74);
	}
};

class AFortWeapon {
public:
	AFortItemDefinition* WeaponData() {
		return (AFortItemDefinition*)(read<uintptr_t>((uintptr_t)this + 0x400));
	}

	AFortItemDefinition* bADSWhileNotOnGround() {
		write<bool>(Settings::MajorValues::LocalPawn + 0x4525, true);
		return 0;
	}
};

class AFortPawnPlayer {
public:
	AFortItemDefinition* WeaponData() {
		return (AFortItemDefinition*)(read<uintptr_t>((uintptr_t)this + 0x400));
	}

	AFortItemDefinition* VehicleGravity() {
		uintptr_t LocalVehicle = read<uintptr_t>(Settings::MajorValues::LocalPawn + Settings::MajorValues::VehicleState);
		write<float>(LocalVehicle + 0x678, 1); //vehicle gravity
		return 0;
	}
};

class APlayerState {
public:
	uintptr_t TeamIndex() {
		return read<uintptr_t>((uintptr_t)this + 0x1020);
	}
};

class ARootComponent {
public:

	FVector RelativeLocation() {
		return read<FVector>((uintptr_t)this + 0x128);
	}
};

class AFortPawn {
public:
	uintptr_t Mesh() {
		return read<uintptr_t>((uintptr_t)this + 0x318);
	}

	AFortWeapon* CurrentWeapon() {
		return (AFortWeapon*)(read<uintptr_t>((uintptr_t)this + 0x7a0));
	}

	AFortPawnPlayer* LocalVehicle() {
		
		return 0;
	}

	ARootComponent* RootComponent() {
		return (ARootComponent*)(read<uintptr_t>((uintptr_t)this + 0x188));
	}

	APlayerState* PlayerState() {
		return (APlayerState*)(read<uintptr_t>((uintptr_t)this + 0x2A0));
	}
};

class APlayerController {
public:
	AFortPawn* LocalPawn() {
		return (AFortPawn*)(read<uintptr_t>((uintptr_t)this + 0x320));
	}
};

class ULocalPlayer {
public:
	APlayerController* PlayerController() {
		return (APlayerController*)(read<uintptr_t>((uintptr_t)this + 0x30));
	}
};

class ULocalPlayers {
public:
	ULocalPlayer* LocalPlayer() {
		return (ULocalPlayer*)(read<uintptr_t>((uintptr_t)this));
	}
};

class UGameInstance {
public:
	ULocalPlayers* LocalPlayers() {
		return (ULocalPlayers*)(read<uintptr_t>((uintptr_t)this + 0x38));
	}
};

class AActors {
public:
	AFortPawn* GetCurrentActor(int i) {
		return (AFortPawn*)(read<uintptr_t>((uintptr_t)this + i * sizeof(uintptr_t)));
	}
};

class ULevel {
public:
	AActors* Actors() {
		return (AActors*)(read<uintptr_t>((uintptr_t)this + 0x98));
	}

	INT ActorCount() {
		return (int)(read<uintptr_t>((uintptr_t)this + 0xA0));
	}

	TArray<AFortPawn*> ActorArray() {
		return read<TArray<AFortPawn*>>((uintptr_t)this + 0x98);
	}
};

class UWorld {
public:

	UGameInstance* OwningGameInstance() {
		return (UGameInstance*)(read<uintptr_t>((uintptr_t)this + 0x190));
	}

	ULevel* PersistentLevel() {
		return (ULevel*)(read<uintptr_t>((uintptr_t)this + 0x30));
	}

	TArray<ULevel*> Levels() {
		return read<TArray<ULevel*>>((uintptr_t)this + 0x148);
	}

};

void gameexploits() {
	auto entityListCopy = entityList;
	for (unsigned long i = 0; i < entityListCopy.size(); ++i) {
		FNlEntity entity = entityListCopy[i];

		if (Settings::Exploits::Backtrack) {
			if (GetAsyncKeyState(VK_RBUTTON))
			{
				write<float>(entity.Actor + 0x64, 0);
				write<float>(Settings::MajorValues::LocalPawn + 0x9C, 1);
			}
			else
			{
				write<float>(entity.Actor + 0x64, 1);
			}
		}

	}
}

float sqrtf_(float _X) {
	return (_mm_cvtss_f32(_mm_sqrt_ps(_mm_set_ss(_X))));
}