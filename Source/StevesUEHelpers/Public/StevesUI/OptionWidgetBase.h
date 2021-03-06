#pragma once

#include "CoreMinimal.h"
#include "FocusableUserWidget.h"
#include "Styling/SlateTypes.h"
#include "StevesHelperCommon.h"
#include "OptionWidgetBase.generated.h"

class UTextBlock;
class UImage;
class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSelectedOptionChanged, class UOptionWidgetBase*, Widget, int, NewIndex);

UCLASS(Abstract, BlueprintType)
class STEVESUEHELPERS_API UOptionWidgetBase : public UFocusableUserWidget
{
    GENERATED_BODY()


public:

    // -- Properties we replicate to child widgets
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Appearance")
    FButtonStyle ButtonStyle;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Appearance")
    FSlateBrush ButtonImage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Appearance")
    FLinearColor ButtonImageColour = FLinearColor::White;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Appearance")
    FSlateFontInfo Font;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Appearance")
    FLinearColor TextColour = FLinearColor::White;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Appearance")
    FSlateBrush TextStrikeBrush;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Appearance")
    FVector2D TextShadowOffset;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Appearance")
    FLinearColor TextShadowColor;

    // -- Properties automatically bound to Blueprint widget
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
    UWidget* MouseVersion;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
    UButton* MouseUpButton;    

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
    UButton* MouseDownButton;    

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
    UImage* MouseUpImage;    

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
    UImage* MouseDownImage;    

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
    UTextBlock* MouseText;    

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
    UButton* GamepadVersion;    

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
    UImage* GamepadUpImage;    

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
    UImage* GamepadDownImage;    

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
    UTextBlock* GamepadText;

    /// Event raised when the selected option changes
    UPROPERTY(BlueprintAssignable)
    FOnSelectedOptionChanged OnSelectedOptionChanged;

    virtual void SynchronizeProperties() override;

    UFUNCTION(BlueprintCallable)
    /// Remove all options
    virtual void ClearOptions();

    /**
     * @brief Adds a new option
     * @param Option The text for the new option
     * @return The index for the new option
     */
    UFUNCTION(BlueprintCallable)
    virtual int AddOption(FText Option);

    
    /**
     * @brief Sets all of the options available for this control
     * @param Options All options to be available
     * @param NewSelectedIndex Which of the options to select by default
     */
    UFUNCTION(BlueprintCallable)
    virtual void SetOptions(const TArray<FText>& Options, int NewSelectedIndex = 0);
    
    UFUNCTION(BlueprintPure)
    virtual int GetSelectedIndex() const { return SelectedIndex; }

    UFUNCTION(BlueprintPure)
    virtual FText GetSelectedOption() const;
    /**
     * @brief Change the selected index option
     * @param NewIndex The new index to set, can be -1 for no selection
     */
    UFUNCTION(BlueprintCallable)
    virtual void SetSelectedIndex(int NewIndex);

    virtual void SetFocusProperly_Implementation() override;

protected:

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Content)
    TArray<FText> Options;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Content)
    int SelectedIndex;

    virtual void SyncButtonProperties(UButton* Button) const;
    virtual void SyncButtonImageProperties(UImage* Img) const;
    virtual void SyncTextProperties(UTextBlock* Txt) const;

    UFUNCTION(BlueprintCallable)
    virtual void SetMouseMode();
    UFUNCTION(BlueprintCallable)
    virtual void SetButtonMode();

    UFUNCTION(BlueprintCallable)
    virtual void UpdateFromInputMode(EInputMode Mode);
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;
    UFUNCTION(BlueprintCallable)
    virtual void ChangeOption(int Delta);
    UFUNCTION(BlueprintCallable)
    virtual EInputMode GetCurrentInputMode() const;


protected:
    UFUNCTION()
    void InputModeChanged(int PlayerIndex, EInputMode NewMode);
    UFUNCTION()
    void MouseUpClicked();
    UFUNCTION()
    void MouseDownClicked();
};
