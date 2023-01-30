class ReflyemWidget extends MovieClip
{
	/* STAGE ELEMENTS */
	
	var healthText	 :TextField;
	var magickaText	 :TextField;
	var staminaText	 :TextField;
	
	var healthValueText	 :TextField;
	var magickaValueText :TextField;
	var staminaValueText :TextField;


	/* INITIALIZATION */

	public function ReflyemWidget()
	{
		super();

		_visible = true;
	}

	/* PRIVATE FUNCTIONS */



	/* PUBLIC FUNCTIONS */

	// @Papyrus
	public function setHealthText(health: String) :Void
	{
		healthText.text = String(health);
	}

	// @Papyrus
	public function setHealthY(posy: Number) :Void
	{
		healthText._y = posy;
	}
	
	// @Papyrus
	public function setHealthX(posx: Number) :Void
	{
		healthText._x = posx;
	}
	
	// @Papyrus
	public function setStaminaText(stamina: String) :Void
	{
		staminaText.text = stamina;
	}
	
	// @Papyrus
	public function setStaminaY(posy: Number) :Void
	{
		staminaText._y = posy;
	}
	
	// @Papyrus
	public function setStaminaX(posx: Number) :Void
	{
		staminaText._x = posx;
	}
	
		// @Papyrus
	public function setMagickaText(magicka: String) :Void
	{
		magickaText.text = magicka;
	}
	// @Papyrus
	public function setMagickaY(posy: Number) :Void
	{
		magickaText._y = posy;
	}
	
	// @Papyrus
	public function setMagickaX(posx: Number) :Void
	{
		magickaText._x = posx;
	}
	
		// @Papyrus
	public function setValueHealthText(health: String) :Void
	{
		healthValueText.text = String(health);
	}

	// @Papyrus
	public function setValueHealthY(posy: Number) :Void
	{
		healthValueText._y = posy;
	}
	
	// @Papyrus
	public function setValueHealthX(posx: Number) :Void
	{
		healthValueText._x = posx;
	}
	
	// @Papyrus
	public function setValueStaminaText(stamina: String) :Void
	{
		staminaValueText.text = stamina;
	}
	
	// @Papyrus
	public function setValueStaminaY(posy: Number) :Void
	{
		staminaValueText._y = posy;
	}
	
	// @Papyrus
	public function setValueStaminaX(posx: Number) :Void
	{
		staminaValueText._x = posx;
	}
	
		// @Papyrus
	public function setValueMagickaText(magicka: String) :Void
	{
		magickaValueText.text = magicka;
	}
	// @Papyrus
	public function setValueMagickaY(posy: Number) :Void
	{
		magickaValueText._y = posy;
	}
	
	// @Papyrus
	public function setValueMagickaX(posx: Number) :Void
	{
		magickaValueText._x = posx;
	}

}
