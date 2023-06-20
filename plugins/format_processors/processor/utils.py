import os


def remove_special_chars(
    text, sep=" ", special_chars="№!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~ "
):
    """
    :param text: to to clean
    :param sep: replacement for special chars
    :param special_chars: special characters to remove
    :return:
    """
    new_text = ""
    start_index = 0
    while start_index < len(text) and text[start_index] in special_chars:
        start_index += 1

    while start_index < len(text):
        if text[start_index] in special_chars:
            while text[start_index] in special_chars:
                start_index += 1
                if start_index >= len(text):
                    return new_text
            new_text += sep
        new_text += text[start_index]
        start_index += 1
    return new_text.strip(sep)


def get_card_image_name(target: str) -> str:
    return f"<img src='{target}'/>"
    # return f"<img src='{os.path.split(saved_image_path)[-1]}'/>"


def get_card_audio_name(target: str) -> str:
    return f"[sound:{target}]"


def get_save_image_name(target) -> str:
    return f"mined-Anki-{remove_special_chars(target, sep='-')}.png"


def get_save_audio_name(target, tags) -> str:
    word = target.strip().lower()
    pos = tags.get("pos")

    raw_audio_name = (
        f"{remove_special_chars(pos, sep='-')}-{remove_special_chars(word, sep='-')}"
        if pos is not None
        else remove_special_chars(word, sep="-")
    )

    audio_name = f"mined-Anki--{raw_audio_name}.mp3"
    return audio_name
